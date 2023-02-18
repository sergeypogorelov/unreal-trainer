// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/EntityEventSubsystem.h"

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRespawnComplete(const int32 SpawnIndex)
{
	if (!OnRespawnCompleteMap.Contains(SpawnIndex))
	{
		const TMulticastDelegate<void()> Delegate;
		OnRespawnCompleteMap.Add(SpawnIndex, Delegate);
	}

	return OnRespawnCompleteMap[SpawnIndex];
}

void UEntityEventSubsystem::Deinitialize()
{
	OnRespawnRequest.Clear();
	OnRoundStart.Clear();
	OnStepStart.Clear();
	OnStepEnd.Clear();
	OnRewardCollected.Clear();
	OnRoundEnd.Clear();

	for (TTuple<int32, TMulticastDelegate<void()>> Pair : OnRespawnCompleteMap)
	{
		Pair.Value.Clear();
	}
	
	Super::Deinitialize();
}
