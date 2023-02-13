﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/EntityRegistrySubsystem.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameShared/PrintUtils.h"

void UEntityRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UEntityRegistrySubsystem::Deinitialize()
{
	EntityMap.Empty();
	
	Super::Deinitialize();
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesByType(const TEnumAsByte<EEntityTypes> EntityType) const
{
	TArray<AActor*> FoundActors;
	if (!EntityMap.Contains(EntityType))
	{
		return FoundActors;
	}

	for (const TWeakObjectPtr<AActor> EntityPtr : EntityMap[EntityType])
	{
		if (EntityPtr.IsValid())
		{
			FoundActors.Add(EntityPtr.Get());
		}
	}

	return FoundActors;
}

void UEntityRegistrySubsystem::RegisterEntity(AActor* Actor)
{
	const IGameEntityInterface* GameEntity = Cast<IGameEntityInterface>(Actor);
	if (GameEntity == nullptr)
	{
		UPrintUtils::PrintAsWarning(TEXT("Attempt to register a not entity"));
		return;
	}

	const TEnumAsByte<EEntityTypes> EntityType = GameEntity->GetEntityType();
	if (!EntityMap.Contains(EntityType))
	{
		EntityMap.Add(EntityType, TArray<TWeakObjectPtr<AActor>>());
	}

	EntityMap[EntityType].Add(Actor);
}

void UEntityRegistrySubsystem::UnregisterEntity(AActor* Actor)
{
	const IGameEntityInterface* GameEntity = Cast<IGameEntityInterface>(Actor);
	if (GameEntity == nullptr)
	{
		UPrintUtils::PrintAsWarning(TEXT("Attempt to unregister a not entity"));
		return;
	}

	const TEnumAsByte<EEntityTypes> EntityType = GameEntity->GetEntityType();
	EntityMap[EntityType].Remove(Actor);
}
