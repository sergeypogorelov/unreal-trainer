// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Enums/EntityTypes.h"
#include "EntityRegistrySubsystem.generated.h"

UCLASS()
class UNREALTRAINER_API UEntityRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	AActor* GetTrainingServer() const;
	AActor* GetGamePlayState(const int32& SpawnIndex) const;
	AActor* GetBot(const int32& SpawnIndex) const;
	TArray<AActor*> GetRewards(const int32& SpawnIndex) const;
	
	AActor* GetEntityBySpawnIndexAndType(const int32& SpawnIndex, const TEnumAsByte<EEntityTypes>& EntityType) const;
	TArray<AActor*> GetEntitiesByType(const TEnumAsByte<EEntityTypes>& EntityType) const;
	TArray<AActor*> GetEntitiesByTypes(const TArray<TEnumAsByte<EEntityTypes>>& EntityTypes) const;
	TArray<AActor*> GetEntitiesBySpawnIndexAndType(const int32& SpawnIndex, const TEnumAsByte<EEntityTypes>& EntityType) const;
	TArray<AActor*> GetEntitiesBySpawnIndexAndTypes(const int32& SpawnIndex, const TArray<TEnumAsByte<EEntityTypes>>& EntityTypes) const;
	
	void RegisterEntity(AActor* Actor);
	void UnregisterEntity(AActor* Actor);
private:
	TMap<TEnumAsByte<EEntityTypes>, TArray<TWeakObjectPtr<AActor>>> EntityMap;
};
