// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/GlobalEventSubsystem.h"
#include "GameShared/Utils/CollectionUtils.h"

TMulticastDelegate<void()>& UGlobalEventSubsystem::OnDynamicEntitiesSpawned(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnDynamicEntitiesSpawnedMap, SpawnIndex);
}

void UGlobalEventSubsystem::Deinitialize()
{
	OnConfigsInitialized.Clear();
	OnStaticEntitiesSpawned.Clear();
	OnDynamicEntitiesSpawnRequest.Clear();
	
	UCollectionUtils::ClearMapWithMulticastDelegates(OnDynamicEntitiesSpawnedMap);
	
	Super::Deinitialize();
}
