// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/Trainer.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Subsystems/GlobalEventSubsystem.h"
#include "GameShared/Utils/PrintUtils.h"
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
			UPrintUtils::PrintAsWarning(TEXT("OnRoundStart"));
			
			BotPtr = Cast<ABotBase>(RegistrySubsystem->GetBot(GetSpawnIndex()));
			BotControllerPtr = Cast<ABotControllerBase>(BotPtr->GetController());

			if (!bIsServerReady)
			{
				return;
			}

			BotPtr->RawMovementComponent->Freeze();
			SendObservations(false);
		});
		EntityEventSubsystem->OnStepStart(GetSpawnIndex()).AddLambda([this]()
		{
			UPrintUtils::PrintAsWarning(TEXT("OnStepStart"));
			
			if (!bIsServerReady)
			{
				return;
			}
			
			Reward = 0;
			Observations.Empty();
			
			BotPtr->RawMovementComponent->Unfreeze();
		});
		EntityEventSubsystem->OnRewardCollected(GetSpawnIndex()).AddLambda([this]()
		{
			UPrintUtils::PrintAsWarning(TEXT("OnRewardCollected"));
			
			if (!bIsServerReady)
			{
				return;
			}
			
			++Reward;
		});
		EntityEventSubsystem->OnStepEnd(GetSpawnIndex()).AddLambda([this]()
		{
			UPrintUtils::PrintAsWarning(TEXT("OnStepEnd"));
			
			if (!bIsServerReady)
			{
				return;
			}
			
			BotPtr->RawMovementComponent->Freeze();
			SendObservations(false);
		});
		EntityEventSubsystem->OnRoundEnd(GetSpawnIndex()).AddLambda([this](const bool bIsVictorious)
		{
			UPrintUtils::PrintAsWarning(TEXT("OnRoundEnd"));
			
			if (!bIsServerReady)
			{
				return;
			}

			BotPtr->RawMovementComponent->Freeze();
			SendObservations(true);
		});
		EntityEventSubsystem->OnTrainingActionReceived(GetSpawnIndex()).AddLambda([this](const FTrainingAction& Action)
		{
			UPrintUtils::PrintAsWarning(TEXT("OnTrainingActionReceived"));
			
			if (!bIsServerReady)
			{
				return;
			}

			BotPtr->MovementDirection = ActionToDirectionMap[Action.Action];
			BotPtr->MovementScale = Action.Action == 0 ? 0 : 1;

			GamePlayStatePtr->CheckForStep();
		});
		EntityEventSubsystem->OnTrainingReset(GetSpawnIndex()).AddLambda([this]()
		{
			UPrintUtils::PrintAsWarning(TEXT("OnTrainingReset"));

			if (!bIsServerReady)
			{
				bIsServerReady = true;
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

void ATrainer::SendObservations(const bool bIsDone)
{
	FTrainingObservations TrainingObservation;

	TrainingObservation.Done = bIsDone;
	TrainingObservation.EnvId = GetSpawnIndex();
	TrainingObservation.Reward = Reward;

	TArray<FString> ObservationsAsStrings;
	for (const float Value : Observations)
	{
		ObservationsAsStrings.Add(FString::SanitizeFloat(Value));
	}
	
	FString ObservationsAsString = UKismetStringLibrary::JoinStringArray(ObservationsAsStrings, TEXT(","));
	TrainingObservation.Observations = FString::Format(TEXT("[{0}]"), { {0, ObservationsAsString }});

	TrainingServerPtr->SendObservations(TrainingObservation);
}
