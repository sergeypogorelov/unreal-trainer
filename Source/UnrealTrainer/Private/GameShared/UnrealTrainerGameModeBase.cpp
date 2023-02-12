// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameShared/UnrealTrainerGameModeBase.h"
#include "GameShared/ConfigRegistrySubsystem.h"
#include "GameShared/EntityEventSubsystem.h"
#include "GameShared/PrintUtils.h"

void AUnrealTrainerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UConfigRegistrySubsystem* ConfigRegistrySubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();

	if (GamePlaySettings)
	{
		ConfigRegistrySubsystem->GamePlaySettingsPtr = GamePlaySettings;
	}
	else
	{
		UPrintUtils::PrintAsError(TEXT("GamePlaySettings are not defined."));
	}

	if (TrainingSettings)
	{
		ConfigRegistrySubsystem->TrainingSettingsPtr = TrainingSettings;
	}
	else
	{
		UPrintUtils::PrintAsError(TEXT("TrainingSettings are not defined."));
	}

	const UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EntityEventSubsystem->OnGameModeBeginPlay.Broadcast();
}
