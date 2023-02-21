// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameShared/UnrealTrainerGameModeBase.h"
#include "GameEntities/GamePlayState.h"
#include "GameEntities/TrainingServer.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Subsystems/GlobalEventSubsystem.h"
#include "GameShared/Utils/PrintUtils.h"
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
		AGamePlayState* GamePlayState = GetWorld()->SpawnActor<AGamePlayState>();
		
		if (GamePlayState == nullptr)
		{
			return false;
		}
		
		GamePlayState->SetSpawnIndex(i);
	}

	GetWorld()->SpawnActor<ATrainingServer>();

	return true;
}

bool AUnrealTrainerGameModeBase::TrySpawnDynamicEntities(const int32 SpawnIndex)
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
	const TArray<AActor*> RegisteredEntities = RegistrySubsystem->GetEntitiesBySpawnIndexExceptByTypes(SpawnIndex, { Area, State });
	for (AActor* Entity : RegisteredEntities)
	{
		Entity->Destroy();
	}

	TArray<AActor*> SpawnedActors;
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
		AActor* SpawnedActor = GetWorld()->SpawnActor(RewardClass, &NewTransform);
		SpawnedActors.Add(SpawnedActor);
	}

	FTransform NewTransform;
	ChangeTransformBySpawnIndex(NewTransform, SpotTransforms[Index], SpawnIndex);
	AActor* SpawnedActor = GetWorld()->SpawnActor(BotClass, &NewTransform);
	SpawnedActors.Add(SpawnedActor);

	for (AActor* Actor : SpawnedActors)
	{
		IGameMultiSpawnInterface* MultiSpawnActor = Cast<IGameMultiSpawnInterface>(Actor);
		MultiSpawnActor->SetSpawnIndex(SpawnIndex);
	}
	
	return true;
}

void AUnrealTrainerGameModeBase::SetUpEventHandlers()
{
	UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EntityEventSubsystem->OnRespawnRequest.AddLambda([this, EntityEventSubsystem](const int32 SpawnIndex)
	{
		if (TrySpawnDynamicEntities(SpawnIndex))
		{
			EntityEventSubsystem->OnRespawnComplete(SpawnIndex).Broadcast();
		}
		else
		{
			UPrintUtils::PrintAsError(TEXT("Failed to respawn entities"));
		}
	});
}

void AUnrealTrainerGameModeBase::ChangeTransformBySpawnIndex(FTransform& NewTransform, const FTransform& OriginTransform,
	const int32 SpawnIndex)
{
	NewTransform = OriginTransform;
	FVector NewLocation(OriginTransform.GetLocation());
	NewLocation.X += ((OriginTrainingAreaLengthOnX + TrainingSettings->AreasOffset) * SpawnIndex);
	NewTransform.SetLocation(NewLocation);
}
