// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalEventSubsystem.generated.h"

UCLASS()
class UNREALTRAINER_API UGlobalEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TMulticastDelegate<void()> OnConfigsInitialized;
	TMulticastDelegate<void()> OnStaticEntitiesSpawned;
	TMulticastDelegate<void(const int32& SpawnIndex)> OnDynamicEntitiesSpawnRequest;
	
	TMulticastDelegate<void()>& OnDynamicEntitiesSpawned(const int32& SpawnIndex);

	virtual void Deinitialize() override;
private:
	TMap<int32, TMulticastDelegate<void()>> OnDynamicEntitiesSpawnedMap;
};
