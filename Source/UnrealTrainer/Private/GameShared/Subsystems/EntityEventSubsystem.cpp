// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/EntityEventSubsystem.h"

void UEntityEventSubsystem::Deinitialize()
{
	OnGameModeBeginPlay.Clear();
	OnRespawnRequest.Clear();
	OnRespawnComplete.Clear();
	OnRoundStart.Clear();
	OnStepStart.Clear();
	OnStepEnd.Clear();
	OnRewardCollected.Clear();
	OnRoundEnd.Clear();
	
	Super::Deinitialize();
}
