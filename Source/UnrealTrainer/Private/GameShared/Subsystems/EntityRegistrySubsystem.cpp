// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameCore/Interfaces/GameMultiSpawnInterface.h"
#include "GameShared/Utils/CollectionUtils.h"
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
	return GetEntityByType(Server);
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

AActor* UEntityRegistrySubsystem::GetEntityByType(const TEnumAsByte<EEntityTypes>& EntityType) const
{
	AActor* FoundActor = nullptr;

	for (const TWeakObjectPtr<AActor>& ActorPtr : EntityMap[0])
	{
		if (!ActorPtr.IsValid())
		{
			continue;
		}
		
		if (Cast<IGameEntityInterface>(ActorPtr.Get())->GetEntityType() == EntityType)
		{
			FoundActor = ActorPtr.Get();
			break;
		}
	}
	
	return FoundActor;
}

AActor* UEntityRegistrySubsystem::GetEntityBySpawnIndexAndType(const int32& SpawnIndex,
                                                               const TEnumAsByte<EEntityTypes>& EntityType) const
{
	AActor* FoundActor = nullptr;

	if (!EntityMap.Contains(SpawnIndex))
	{
		return nullptr;
	}

	for (const TWeakObjectPtr<AActor>& ActorPtr : EntityMap[SpawnIndex])
	{
		if (!ActorPtr.IsValid())
		{
			continue;
		}
		
		if (Cast<IGameEntityInterface>(ActorPtr.Get())->GetEntityType() == EntityType)
		{
			FoundActor = ActorPtr.Get();
			break;
		}
	}
	
	return FoundActor;
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesByType(const TEnumAsByte<EEntityTypes>& EntityType) const
{
	return GetEntitiesByTypes({ EntityType });
}

TArray<AActor*> UEntityRegistrySubsystem::GetEntitiesByTypes(const TArray<TEnumAsByte<EEntityTypes>>& EntityTypes) const
{
	TArray<AActor*> FoundActors;
	
	for (const TTuple<int32, TArray<TWeakObjectPtr<AActor>>>& Pair : EntityMap)
	{
		for (const TWeakObjectPtr<AActor>& ActorPtr : Pair.Value)
		{
			if (!ActorPtr.IsValid())
			{
				continue;
			}
			
			if (EntityTypes.Contains(Cast<IGameEntityInterface>(ActorPtr.Get())->GetEntityType()))
			{
				FoundActors.Add(ActorPtr.Get());
			}
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

	if (!EntityMap.Contains(SpawnIndex))
	{
		return FoundActors;
	}

	for (const TWeakObjectPtr<AActor>& ActorPtr : EntityMap[SpawnIndex])
	{
		if (!ActorPtr.IsValid())
		{
			continue;
		}
		
		if (EntityTypes.Contains(Cast<IGameEntityInterface>(ActorPtr.Get())->GetEntityType()))
		{
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

	int32 SpawnIndex = 0;
	const IGameMultiSpawnInterface* MultiSpawnEntity = Cast<IGameMultiSpawnInterface>(Actor);

	if (MultiSpawnEntity != nullptr)
	{
		SpawnIndex = MultiSpawnEntity->GetSpawnIndex();
	}

	UCollectionUtils::GetMapValue(EntityMap, SpawnIndex).Add(Actor);
}

void UEntityRegistrySubsystem::UnregisterEntity(AActor* Actor)
{
	const IGameEntityInterface* GameEntity = Cast<IGameEntityInterface>(Actor);
	if (GameEntity == nullptr)
	{
		UPrintUtils::PrintAsWarning(TEXT("Attempt to unregister a not entity"));
		return;
	}

	int32 SpawnIndex = 0;
	const IGameMultiSpawnInterface* MultiSpawnEntity = Cast<IGameMultiSpawnInterface>(Actor);

	if (MultiSpawnEntity != nullptr)
	{
		SpawnIndex = MultiSpawnEntity->GetSpawnIndex();
	}

	UCollectionUtils::GetMapValue(EntityMap, SpawnIndex).Remove(Actor);
}
