// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameShared/UnrealTrainerGameModeBase.h"
#include "GameShared/ConfigRegistrySubsystem.h"
#include "GameShared/EntityEventSubsystem.h"
#include "GameShared/PrintUtils.h"

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

	const UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EntityEventSubsystem->OnGameModeBeginPlay.Broadcast();
}
