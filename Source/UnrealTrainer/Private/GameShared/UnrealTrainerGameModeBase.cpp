// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameShared/UnrealTrainerGameModeBase.h"
#include "GameEntities/GamePlayState.h"
#include "GameEntities/Trainer.h"
#include "GameEntities/TrainingServer.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Subsystems/GlobalEventSubsystem.h"
#include "GameShared/Utils/PrintUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AUnrealTrainerGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetUpEventHandlers();
	
	if (!TryInitConfigs()) return;

	const UGlobalEventSubsystem* GlobalEventSubsystem = GetGameInstance()->GetSubsystem<UGlobalEventSubsystem>();
	GlobalEventSubsystem->OnConfigsInitialized.Broadcast();
	
	if (!TryInitOriginTrainingArea()) return;
	if (!TryInitOriginSpots()) return;
	if (!TrySpawnTrainingAreas()) return;
	if (!TrySpawnStaticEntities()) return;

	GlobalEventSubsystem->OnStaticEntitiesSpawned.Broadcast();
}

bool AUnrealTrainerGameModeBase::TryInitConfigs()
{
	if (GamePlaySettings == nullptr)
	{
		UPrintUtils::PrintAsError(TEXT("GamePlaySettings are not specified"));
		return false;
	}

	if (TrainingSettings == nullptr)
	{
		UPrintUtils::PrintAsError(TEXT("TrainingSettings are not specified"));
		return false;
	}

	UConfigRegistrySubsystem* ConfigRegistrySubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
	
	ConfigRegistrySubsystem->GamePlaySettingsPtr = GamePlaySettings;
	ConfigRegistrySubsystem->TrainingSettingsPtr = TrainingSettings;

	return true;
}

bool AUnrealTrainerGameModeBase::TryInitOriginTrainingArea()
{
	const UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	const TArray<AActor*> RegisteredEntities = RegistrySubsystem->GetEntitiesByType(Area);
	
	if (RegisteredEntities.Num() == 0)
	{
		UPrintUtils::PrintAsError(TEXT("Training area is not found in registry"));
		return false;
	}

	if (RegisteredEntities.Num() > 1)
	{
		UPrintUtils::PrintAsError(TEXT("There are few training areas in registry"));
		return false;
	}

	OriginTrainingAreaPtr = Cast<ATrainingAreaBase>(RegisteredEntities.Last());
	OriginTrainingAreaLengthOnX = OriginTrainingAreaPtr->GetLengthOnX();
	OriginTrainingAreaLengthOnY = OriginTrainingAreaPtr->GetLengthOnY();
	OriginTrainingAreaTransform = OriginTrainingAreaPtr->GetActorTransform();
	
	return true;
}

bool AUnrealTrainerGameModeBase::TryInitOriginSpots()
{
	const UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	const TArray<AActor*> RegisteredEntities = RegistrySubsystem->GetEntitiesByType(Spot);

	if (RegisteredEntities.Num() < 2)
	{
		UPrintUtils::PrintAsError(TEXT("There is not enough spots found in registry"));
		return false;
	}

	for (AActor* Entity : RegisteredEntities)
	{
		OriginSpotTransforms.Add(Entity->GetActorTransform());
		Entity->Destroy();
	}
	
	return true;
}

bool AUnrealTrainerGameModeBase::TrySpawnTrainingAreas()
{
	if (TrainingAreaClass == nullptr)
	{
		UPrintUtils::PrintAsError(TEXT("Training area class is not specified"));
		return false;
	}

	for (int32 i = 1; i < TrainingSettings->CountOfAgents; ++i)
	{
		FTransform NewTransform;
		ChangeTransformBySpawnIndex(NewTransform, OriginTrainingAreaTransform, i);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Template = OriginTrainingAreaPtr.Get();
		GetWorld()->SpawnActor(TrainingAreaClass, &NewTransform, SpawnParameters);
	}

	return true;
}

bool AUnrealTrainerGameModeBase::TrySpawnStaticEntities()
{
	for (int32 i = 0; i < TrainingSettings->CountOfAgents; ++i)
	{
		AGamePlayState* GamePlayState = GetWorld()->SpawnActorDeferred<AGamePlayState>(AGamePlayState::StaticClass(), FTransform());
		ATrainer* Trainer = GetWorld()->SpawnActorDeferred<ATrainer>(ATrainer::StaticClass(), FTransform());
		
		if (GamePlayState == nullptr || Trainer == nullptr)
		{
			return false;
		}
		
		GamePlayState->SetSpawnIndex(i);
		Trainer->SetSpawnIndex(i);

		UGameplayStatics::FinishSpawningActor(GamePlayState, FTransform());
		UGameplayStatics::FinishSpawningActor(Trainer, FTransform());
	}

	GetWorld()->SpawnActor<ATrainingServer>();

	return true;
}

bool AUnrealTrainerGameModeBase::TrySpawnDynamicEntities(const int32& SpawnIndex)
{
	if (BotClass == nullptr)
	{
		UPrintUtils::PrintAsError(TEXT("Bot class is not specified"));
		return false;
	}

	if (RewardClass == nullptr)
	{
		UPrintUtils::PrintAsError(TEXT("Reward class is not specified"));
		return false;
	}

	if (OriginSpotTransforms.Num() < GamePlaySettings->CountOfRewardsToSpawn + 1)
	{
		UPrintUtils::PrintAsError(TEXT("There is not enough spots to spawn entities"));
		return false;
	}
	
	const UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	const TArray<AActor*> RegisteredEntities = RegistrySubsystem->GetEntitiesBySpawnIndexAndTypes(SpawnIndex, { Bot, Reward });
	for (AActor* Entity : RegisteredEntities)
	{
		Entity->Destroy();
	}

	TArray<FTransform> SpotTransforms = OriginSpotTransforms;
	if (GamePlaySettings->bShouldRandomize)
	{
		SpotTransforms.Sort([](const FTransform& A, const FTransform& B)
		{
			return UKismetMathLibrary::RandomBool();
		});
	}

	int32 Index = 0;
	for (int32 i = 0; i < GamePlaySettings->CountOfRewardsToSpawn; ++Index, ++i)
	{
		FTransform NewTransform;
		ChangeTransformBySpawnIndex(NewTransform, SpotTransforms[Index], SpawnIndex);
		ARewardBase* SpawnedReward = GetWorld()->SpawnActorDeferred<ARewardBase>(RewardClass, NewTransform);
		SpawnedReward->SetSpawnIndex(SpawnIndex);
		UGameplayStatics::FinishSpawningActor(SpawnedReward, NewTransform);
	}

	FTransform NewTransform;
	ChangeTransformBySpawnIndex(NewTransform, SpotTransforms[Index], SpawnIndex);
	ABotBase* SpawnedBot = GetWorld()->SpawnActorDeferred<ABotBase>(BotClass, NewTransform);
	SpawnedBot->SetSpawnIndex(SpawnIndex);
	UGameplayStatics::FinishSpawningActor(SpawnedBot, NewTransform);
	
	return true;
}

void AUnrealTrainerGameModeBase::SetUpEventHandlers()
{
	UGlobalEventSubsystem* GlobalEventSubsystem = GetGameInstance()->GetSubsystem<UGlobalEventSubsystem>();
	GlobalEventSubsystem->OnDynamicEntitiesSpawnRequest.AddLambda([this, GlobalEventSubsystem](const int32& SpawnIndex)
	{
		if (TrySpawnDynamicEntities(SpawnIndex))
		{
			GlobalEventSubsystem->OnDynamicEntitiesSpawned(SpawnIndex).Broadcast();
		}
		else
		{
			UPrintUtils::PrintAsError(TEXT("Failed to respawn entities"));
		}
	});
}

void AUnrealTrainerGameModeBase::ChangeTransformBySpawnIndex(FTransform& NewTransform, const FTransform& OriginTransform,
	const int32& SpawnIndex)
{
	NewTransform = OriginTransform;
	FVector NewLocation(OriginTransform.GetLocation());
	NewLocation.X += ((OriginTrainingAreaLengthOnX + TrainingSettings->AreasOffset) * SpawnIndex);
	NewTransform.SetLocation(NewLocation);
}
