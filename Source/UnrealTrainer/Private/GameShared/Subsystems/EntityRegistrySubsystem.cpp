// Fill out your copyright notice in the Description page of Project Settings.

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
	if (!EntityMap.Contains(Server))
	{
		return nullptr;
	}

	if (EntityMap[Server].Num() == 0)
	{
		return nullptr;
	}
	
	const TWeakObjectPtr<AActor>& ActorPtr = EntityMap[Server][0];
	return ActorPtr.IsValid() ? ActorPtr.Get() : nullptr;
}

AActor* UEntityRegistrySubsystem::GetGamePlayState(const int32& SpawnIndex) const
{
	return GetEntityBySpawnIndexAndType(SpawnIndex, State);
}

AActor* UEntityRegistrySubsystem::GetBot(const int32& SpawnIndex) const
{
	return GetEntityBySpawnIndexAndType(SpawnIndex, Bot);
}

TArray<AActor*> UEntityRegistrySubsystem::GetRewards(const int32& SpawnIndex) const
{
	return GetEntitiesBySpawnIndexAndType(SpawnIndex, Reward);
}

AActor* UEntityRegistrySubsystem::GetEntityBySpawnIndexAndType(const int32& SpawnIndex,
	const TEnumAsByte<EEntityTypes>& EntityType) const
{
	if (!EntityMap.Contains(EntityType))
	{
		return nullptr;
	}

	for (const TWeakObjectPtr<AActor>& ActorPtr : EntityMap[EntityType])
	{
		if (!ActorPtr.IsValid())
		{
			continue;
		}

		const IGameMultiSpawnInterface* GameMultiSpawnEntity = Cast<IGameMultiSpawnInterface>(ActorPtr.Get());
		if (GameMultiSpawnEntity == nullptr)
		{
			continue;
		}

		if (GameMultiSpawnEntity->GetSpawnIndex() == SpawnIndex)
		{
			return ActorPtr.Get();
		}
	}
	
	return nullptr;
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesByType(const TEnumAsByte<EEntityTypes>& EntityType) const
{
	return GetEntitiesByTypes({ EntityType });
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesByTypes(const TArray<TEnumAsByte<EEntityTypes>>& EntityTypes) const
{
	TArray<AActor*> FoundActors;
	
	for (const TTuple<TEnumAsByte<EEntityTypes>, TArray<TWeakObjectPtr<AActor>>>& Pair : EntityMap)
	{
		if (!EntityTypes.Contains(Pair.Key))
		{
			continue;
		}

		for (const TWeakObjectPtr<AActor>& ActorPtr : Pair.Value)
		{
			if (!ActorPtr.IsValid())
			{
				continue;
			}

			FoundActors.Add(ActorPtr.Get());
		}
	}

	return FoundActors;
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesBySpawnIndexAndType(const int32& SpawnIndex,
	const TEnumAsByte<EEntityTypes>& EntityType) const
{
	return GetEntitiesBySpawnIndexAndTypes(SpawnIndex, { EntityType });
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesBySpawnIndexAndTypes(const int32& SpawnIndex,
	const TArray<TEnumAsByte<EEntityTypes>>& EntityTypes) const
{
	TArray<AActor*> FoundActors;

	for (const TTuple<TEnumAsByte<EEntityTypes>, TArray<TWeakObjectPtr<AActor>>>& Pair : EntityMap)
	{
		if (!EntityTypes.Contains(Pair.Key))
		{
			continue;
		}

		for (const TWeakObjectPtr<AActor>& ActorPtr : Pair.Value)
		{
			if (!ActorPtr.IsValid())
			{
				continue;
			}

			const IGameMultiSpawnInterface* MultiSpawnActor = Cast<IGameMultiSpawnInterface>(ActorPtr.Get());
			if (MultiSpawnActor == nullptr)
			{
				continue;
			}

			if (MultiSpawnActor->GetSpawnIndex() != SpawnIndex)
			{
				continue;
			}

			FoundActors.Add(ActorPtr.Get());
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
