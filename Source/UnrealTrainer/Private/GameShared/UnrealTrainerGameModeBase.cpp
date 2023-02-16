// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameShared/UnrealTrainerGameModeBase.h"
#include "GameEntities/GamePlayState.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Utils/PrintUtils.h"
#include "Kismet/KismetMathLibrary.h"

void AUnrealTrainerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!TryInitConfigs()) return;
	if (!TryInitOriginTrainingArea()) return;
	if (!TryInitOriginSpots()) return;
	if (!TrySpawnTrainingAreas()) return;
	if (!TrySpawnStaticEntities()) return;

	SetUpEventHandlers();
	
	const UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EntityEventSubsystem->OnGameModeBeginPlay.Broadcast();
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
		FTransform NewTransform(OriginTrainingAreaTransform);
		FVector NewLocation(OriginTrainingAreaTransform.GetLocation());
		NewLocation.X += ((OriginTrainingAreaLengthOnX + TrainingSettings->AreasOffset) * i);
		NewTransform.SetLocation(NewLocation);

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
		GetWorld()->SpawnActor<AGamePlayState>();
	}

	return true;
}

bool AUnrealTrainerGameModeBase::TrySpawnDynamicEntities()
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
	const TArray<AActor*> RegisteredEntities = RegistrySubsystem->GetEntitiesExceptByType(Area);
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
		GetWorld()->SpawnActor(RewardClass, &SpotTransforms[Index]);
		
	}

	GetWorld()->SpawnActor(BotClass, &SpotTransforms[Index]);
	
	return true;
}

void AUnrealTrainerGameModeBase::SetUpEventHandlers()
{
	UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EntityEventSubsystem->OnRespawnRequest.AddLambda([this, EntityEventSubsystem]()
	{
		if (TrySpawnDynamicEntities())
		{
			EntityEventSubsystem->OnRespawnComplete.Broadcast();
		}
		else
		{
			UPrintUtils::PrintAsError(TEXT("Failed to respawn entities"));
		}
	});
}
