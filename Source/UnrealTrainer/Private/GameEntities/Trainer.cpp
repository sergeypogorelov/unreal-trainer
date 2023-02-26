// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/Trainer.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Subsystems/GlobalEventSubsystem.h"
#include "GameShared/Utils/MathUtils.h"
#include "GameShared/Utils/PrintUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ATrainer::ATrainer()
{
	PrimaryActorTick.bCanEverTick = true;
}

TEnumAsByte<EEntityTypes> ATrainer::GetEntityType() const
{
	return Trainer;
}

int32 ATrainer::GetSpawnIndex() const
{
	return SpawnIndex;
}

void ATrainer::SetSpawnIndex(const int32 SpawnIndexVar)
{
	SpawnIndex = SpawnIndexVar;
}

void ATrainer::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	RegistrySubsystem->RegisterEntity(this);
	
	UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	UGlobalEventSubsystem* GlobalEventSubsystem = GetGameInstance()->GetSubsystem<UGlobalEventSubsystem>();
	
	GlobalEventSubsystem->OnStaticEntitiesSpawned.AddLambda([this, RegistrySubsystem, EntityEventSubsystem]()
	{
		TrainingServerPtr = Cast<ATrainingServer>(RegistrySubsystem->GetTrainingServer());
		GamePlayStatePtr = Cast<AGamePlayState>(RegistrySubsystem->GetGamePlayState(GetSpawnIndex()));

		EntityEventSubsystem->OnRoundStart(GetSpawnIndex()).AddLambda([this, RegistrySubsystem]()
		{
			UpdateDynamicEntityPtrs();
			
			if (!bIsServerReady)
			{
				return;
			}

			if (!BotPtr.IsValid())
			{
				UPrintUtils::PrintAsError(TEXT("On round start the bot is not valid"));
				return;
			}
			
			BotPtr->RawMovementComponent->Freeze();
			SendObservations(0, false);
		});
		EntityEventSubsystem->OnStepStart(GetSpawnIndex()).AddLambda([this]()
		{
			if (!bIsServerReady)
			{
				return;
			}

			if (!BotPtr.IsValid())
			{
				UPrintUtils::PrintAsError(TEXT("On step start the bot is not valid"));
				return;
			}
			
			BotPtr->RawMovementComponent->Unfreeze();
		});
		EntityEventSubsystem->OnStepEnd(GetSpawnIndex()).AddLambda([this](const int32 RewardsCollected)
		{
			if (!bIsServerReady)
			{
				return;
			}

			if (!BotPtr.IsValid())
			{
				UPrintUtils::PrintAsError(TEXT("On step end the bot is not valid"));
				return;
			}
			
			BotPtr->RawMovementComponent->Freeze();
			SendObservations(RewardsCollected, false);
		});
		EntityEventSubsystem->OnRoundEnd(GetSpawnIndex()).AddLambda([this](const bool bIsVictorious, const int32 RewardsCollected)
		{
			if (!bIsServerReady)
			{
				return;
			}

			if (!BotPtr.IsValid())
			{
				UPrintUtils::PrintAsError(TEXT("On round end the bot is not valid"));
				return;
			}
			
			BotPtr->RawMovementComponent->Freeze();
			SendObservations(RewardsCollected, true);
		});
		EntityEventSubsystem->OnTrainingActionReceived(GetSpawnIndex()).AddLambda([this](const FTrainingAction& Action)
		{
			if (!bIsServerReady)
			{
				return;
			}

			if (!BotPtr.IsValid())
			{
				UPrintUtils::PrintAsError(TEXT("Not possible to set action as the bot is not valid"));
				return;
			}
			
			BotPtr->MovementDirection = ActionToDirectionMap[Action.Action];
			BotPtr->MovementScale = Action.Action == 0 ? 0 : 1;

			if (!GamePlayStatePtr.IsValid())
			{
				UPrintUtils::PrintAsError(TEXT("Not possible to check step as game play state is not valid"));
				return;
			}
			
			GamePlayStatePtr->CheckStep();
		});
		EntityEventSubsystem->OnTrainingReset(GetSpawnIndex()).AddLambda([this]()
		{
			if (!bIsServerReady)
			{
				bIsServerReady = true;
			}

			if (!GamePlayStatePtr.IsValid())
			{
				UPrintUtils::PrintAsError(TEXT("Not possible to request for restart as game play state is not valid"));
				return;
			}
			
			GamePlayStatePtr->RequestForRestart();
		});
	});
}

void ATrainer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}

TArray<TWeakObjectPtr<ARewardBase>> ATrainer::GetValidRewardPtrs() const
{
	return RewardPtrs.FilterByPredicate([](TWeakObjectPtr<ARewardBase> RewardPtr)
	{
		return RewardPtr.IsValid();
	});
}

void ATrainer::SendObservations(const int32 Rewards, const bool bIsDone)
{
	FTrainingObservations TrainingObservation;

	TrainingObservation.Done = bIsDone;
	TrainingObservation.EnvId = GetSpawnIndex();
	TrainingObservation.Reward = Rewards;

	float Angle = CalcAngleToClosestReward();
	
	const UConfigRegistrySubsystem* ConfigRegistrySubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
	const TWeakObjectPtr<UTrainingSettings> TrainingSettingsPtr = ConfigRegistrySubsystem->TrainingSettingsPtr;
	if (TrainingSettingsPtr->bShouldNormalize)
	{
		Angle = UKismetMathLibrary::NormalizeToRange(Angle, 0, 360);
	}
	
	TArray<float> Observations;
	Observations.Add(Angle);
	
	TArray<FString> ObservationsAsStrings;
	for (const float& Value : Observations)
	{
		ObservationsAsStrings.Add(FString::SanitizeFloat(Value));
	}
	
	FString ObservationsAsString = UKismetStringLibrary::JoinStringArray(ObservationsAsStrings, TEXT(","));
	TrainingObservation.Observations = FString::Format(TEXT("[{0}]"), { ObservationsAsString });

	if (!TrainingServerPtr.IsValid())
	{
		UPrintUtils::PrintAsError(TEXT("Not possible to send observations as training server is not valid"));
		return;
	}
	
	TrainingServerPtr->SendObservations(TrainingObservation);
}

void ATrainer::UpdateDynamicEntityPtrs()
{
	const UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	
	BotPtr = Cast<ABotBase>(RegistrySubsystem->GetBot(GetSpawnIndex()));
	BotControllerPtr = Cast<ABotControllerBase>(BotPtr->GetController());

	UpdateRewardPtrs();
}

void ATrainer::UpdateRewardPtrs()
{
	RewardPtrs.Empty();
	
	const UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	TArray<AActor*> RewardActors = RegistrySubsystem->GetRewards(GetSpawnIndex());
	for (AActor* Actor : RewardActors)
	{
		TWeakObjectPtr<ARewardBase> ActorPtr = Cast<ARewardBase>(Actor);
		RewardPtrs.Add(ActorPtr);
	}
}

float ATrainer::CalcAngleToClosestReward() const
{
	const FVector BotLocation = BotPtr->GetActorLocation();
	const FVector2D BotLocation2D = FVector2D(BotLocation.X, BotLocation.Y);

	const TArray<TWeakObjectPtr<ARewardBase>> ValidRewardPtrs = GetValidRewardPtrs();
	
	int32 ClosetRewardIndex = 0;
	FVector RewardLocation = ValidRewardPtrs[ClosetRewardIndex]->GetActorLocation();
	FVector2D RewardLocation2D = FVector2D(RewardLocation.X, RewardLocation.Y);
	float MinDistance = UKismetMathLibrary::Distance2D(BotLocation2D, RewardLocation2D);
	
	for (int32 i = 1; i < ValidRewardPtrs.Num(); ++i)
	{
		RewardLocation = ValidRewardPtrs[i]->GetActorLocation();
		RewardLocation2D = FVector2D(RewardLocation.X, RewardLocation.Y);
		const float Distance = UKismetMathLibrary::Distance2D(BotLocation2D, RewardLocation2D);
		
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosetRewardIndex = i;
		}
	}

	const FVector ClosestRewardLocation = ValidRewardPtrs[ClosetRewardIndex]->GetActorLocation();
	const FVector2D ClosestRewardLocation2D = FVector2D(ClosestRewardLocation.X, ClosestRewardLocation.Y);
	return UMathUtils::CalcAngleBetweenLocations(BotLocation2D, ClosestRewardLocation2D);
}
