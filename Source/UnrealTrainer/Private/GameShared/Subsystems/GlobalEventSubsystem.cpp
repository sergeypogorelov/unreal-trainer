// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/GlobalEventSubsystem.h"

void UGlobalEventSubsystem::Deinitialize()
{
	OnConfigsInitialized.Clear();
	OnStaticEntitiesSpawned.Clear();
	
	Super::Deinitialize();
}
