// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/EntityEventSubsystem.h"

void UEntityEventSubsystem::Deinitialize()
{
	OnGameModeBeginPlay.Clear();
	
	Super::Deinitialize();
}
