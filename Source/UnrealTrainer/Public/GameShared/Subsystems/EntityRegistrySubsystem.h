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

	TArray<AActor*> GetEntitiesByType(TEnumAsByte<EEntityTypes> EntityType) const;
	TArray<AActor*> GetEntitiesExceptByType(TEnumAsByte<EEntityTypes> EntityType) const;
	
	void RegisterEntity(AActor* Actor);
	void UnregisterEntity(AActor* Actor);
private:
	TMap<TEnumAsByte<EEntityTypes>, TArray<TWeakObjectPtr<AActor>>> EntityMap;
};
