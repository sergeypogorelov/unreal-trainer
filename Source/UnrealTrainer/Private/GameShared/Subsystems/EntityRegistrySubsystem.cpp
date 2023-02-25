﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameCore/Interfaces/GameMultiSpawnInterface.h"
#include "GameShared/Utils/PrintUtils.h"

void UEntityRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UEntityRegistrySubsystem::Deinitialize()
{
	EntityMap.Empty();
	
	Super::Deinitialize();
}

AActor* UEntityRegistrySubsystem::GetTrainingServer() const
{
	TArray<AActor*> FoundActors = GetEntitiesByType(Server);
	return FoundActors.Num() > 0 ? FoundActors[0] : nullptr;
}

AActor* UEntityRegistrySubsystem::GetGamePlayState(const int32 SpawnIndex) const
{
	return GetEntityBySpawnIndexAndType(SpawnIndex, State);
}

AActor* UEntityRegistrySubsystem::GetBot(const int32 SpawnIndex) const
{
	return GetEntityBySpawnIndexAndType(SpawnIndex, Bot);
}

TArray<AActor*> UEntityRegistrySubsystem::GetRewards(const int32& SpawnIndex) const
{
	return GetEntitiesBySpawnIndexAndType(SpawnIndex, Reward);
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

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesBySpawnIndexAndType(const int32& SpawnIndex,
	const TEnumAsByte<EEntityTypes>& EntityType) const
{
	TArray<AActor*> FoundActors;

	if (!EntityMap.Contains(EntityType))
	{
		return FoundActors;
	}

	TArray<TWeakObjectPtr<AActor>> FilteredActorPtrs = EntityMap[EntityType].FilterByPredicate([&SpawnIndex](const TWeakObjectPtr<AActor>& ActorPtr)
	{
		if (!ActorPtr.IsValid())
		{
			return false;
		}

		const IGameMultiSpawnInterface* GameMultiSpawnEntity = Cast<IGameMultiSpawnInterface>(ActorPtr.Get());
		if (GameMultiSpawnEntity == nullptr)
		{
			return false;
		}

		return GameMultiSpawnEntity->GetSpawnIndex() == SpawnIndex;
	});

	for (const TWeakObjectPtr<AActor>& ActorPtr : FilteredActorPtrs)
	{
		FoundActors.Add(ActorPtr.Get());
	}
	
	return FoundActors;
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesExceptByType(TEnumAsByte<EEntityTypes> EntityType) const
{
	return GetEntitiesExceptByTypes({ EntityType });
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesExceptByTypes(TArray<TEnumAsByte<EEntityTypes>> EntityTypes) const
{
	TArray<AActor*> FoundActors;
	
	for (const TTuple<TEnumAsByte<EEntityTypes>, TArray<TWeakObjectPtr<AActor>>> Pair : EntityMap)
	{
		if (EntityTypes.Contains(Pair.Key))
		{
			continue;
		}

		for (const TWeakObjectPtr<AActor> ActorPtr : Pair.Value)
		{
			if (ActorPtr.IsValid())
			{
				FoundActors.Add(ActorPtr.Get());
			}
		}
	}

	return FoundActors;
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesBySpawnIndexExceptByType(const int32 SpawnIndex,
	TEnumAsByte<EEntityTypes> EntityType) const
{
	return GetEntitiesBySpawnIndexExceptByTypes(SpawnIndex, { EntityType });
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesBySpawnIndexExceptByTypes(const int32 SpawnIndex,
	TArray<TEnumAsByte<EEntityTypes>> EntityTypes) const
{
	const TArray<AActor*> FoundActors = GetEntitiesExceptByTypes(EntityTypes);
	return FoundActors.FilterByPredicate([SpawnIndex](AActor* CurrentActor)
	{
		const IGameMultiSpawnInterface* MultiSpawnActor = Cast<IGameMultiSpawnInterface>(CurrentActor);
		return MultiSpawnActor != nullptr && MultiSpawnActor->GetSpawnIndex() == SpawnIndex;
	});
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

AActor* UEntityRegistrySubsystem::GetEntityBySpawnIndexAndType(const int32 SpawnIndex,
	const TEnumAsByte<EEntityTypes> EntityType) const
{
	TArray<AActor*> FoundActors = GetEntitiesByType(EntityType).FilterByPredicate([SpawnIndex](AActor* Actor)
	{
		const IGameMultiSpawnInterface* MultiSpawnActor = Cast<IGameMultiSpawnInterface>(Actor);
		return MultiSpawnActor->GetSpawnIndex() == SpawnIndex;
	});
	
	return FoundActors.Num() > 0 ? FoundActors[0] : nullptr;
}
