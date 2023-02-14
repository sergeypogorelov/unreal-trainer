// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameShared/UnrealTrainerGameModeBase.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Utils/PrintUtils.h"

void AUnrealTrainerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UConfigRegistrySubsystem* ConfigRegistrySubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();

	if (GamePlaySettings == nullptr)
	{
		UPrintUtils::PrintAsError(TEXT("GamePlaySettings are not defined."));
		return;
	}

	if (TrainingSettings == nullptr)
	{
		UPrintUtils::PrintAsError(TEXT("TrainingSettings are not defined."));
		return;
	}

	ConfigRegistrySubsystem->GamePlaySettingsPtr = GamePlaySettings;
	ConfigRegistrySubsystem->TrainingSettingsPtr = TrainingSettings;

	if (!SpawnTrainingAreas())
	{
		return;
	}

	const UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EntityEventSubsystem->OnGameModeBeginPlay.Broadcast();
}

bool AUnrealTrainerGameModeBase::SpawnTrainingAreas()
{
	if (TrainingAreaClass == nullptr)
	{
		UPrintUtils::PrintAsError(TEXT("Training area class is not specified"));
		return false;
	}
	
	const UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	const TArray<AActor*> RegisteredEntities = RegistrySubsystem->GetEntitiesByType(Area);
	if (RegisteredEntities.Num() == 0)
	{
		UPrintUtils::PrintAsError(TEXT("Training area is not found in registry"));
		return false;
	}
	
	ATrainingAreaBase* TrainingArea = Cast<ATrainingAreaBase>(RegisteredEntities.Last());
	const FTransform OriginTransform = TrainingArea->GetActorTransform();

	for (int32 i = 1; i < TrainingSettings->CountOfAgents; ++i)
	{
		FTransform NewTransform(OriginTransform);
		FVector NewLocation(OriginTransform.GetLocation());
		NewLocation.X += ((TrainingArea->GetLengthOnX() + TrainingSettings->AreasOffset) * i);
		NewTransform.SetLocation(NewLocation);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Template = TrainingArea;
		GetWorld()->SpawnActor(TrainingAreaClass, &NewTransform, SpawnParameters);
	}

	return true;
}
