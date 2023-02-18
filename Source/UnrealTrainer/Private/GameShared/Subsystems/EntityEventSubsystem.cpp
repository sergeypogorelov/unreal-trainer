// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/EntityEventSubsystem.h"

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRespawnComplete(const int32 SpawnIndex)
{
	return OnCommonEventWithSpawnIndex(OnRespawnCompleteMap, SpawnIndex);
}

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRoundStart(const int32 SpawnIndex)
{
	return OnCommonEventWithSpawnIndex(OnRoundStartMap, SpawnIndex);
}

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRewardCollected(const int32 SpawnIndex)
{
	return OnCommonEventWithSpawnIndex(OnRewardCollectedMap, SpawnIndex);
}

TMulticastDelegate<void(const bool bIsVictorious)>& UEntityEventSubsystem::OnRoundEnd(const int32 SpawnIndex)
{
	if (!OnRoundEndMap.Contains(SpawnIndex))
	{
		const TMulticastDelegate<void(const bool bIsVictorious)> Delegate;
		OnRoundEndMap.Add(SpawnIndex, Delegate);
	}

	return OnRoundEndMap[SpawnIndex];
}

void UEntityEventSubsystem::Deinitialize()
{
	/// TODO: add others
	OnRespawnRequest.Clear();
	
	Super::Deinitialize();
}

TMulticastDelegate<void()>& UEntityEventSubsystem::OnCommonEventWithSpawnIndex(
	TMap<int32, TMulticastDelegate<void()>>& Map, const int32 SpawnIndex)
{
	/// TODO: replace with template method
	if (!Map.Contains(SpawnIndex))
	{
		const TMulticastDelegate<void()> Delegate;
		Map.Add(SpawnIndex, Delegate);
	}

	return Map[SpawnIndex];
}
