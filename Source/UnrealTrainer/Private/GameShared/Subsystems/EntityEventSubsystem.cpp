// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Utils/CollectionUtils.h"

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRespawnComplete(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnRespawnCompleteMap, SpawnIndex);
}

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRoundStart(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnRoundStartMap, SpawnIndex);
}

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRewardCollected(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnRewardCollectedMap, SpawnIndex);
}

TMulticastDelegate<void(const bool bIsVictorious)>& UEntityEventSubsystem::OnRoundEnd(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnRoundEndMap, SpawnIndex);
}

TMulticastDelegate<void(const FTrainingAction& Action)>& UEntityEventSubsystem::OnTrainingActionReceived(
	const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnTrainingActionReceivedMap, SpawnIndex);
}

TMulticastDelegate<void()>& UEntityEventSubsystem::OnTrainingReset(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnTrainingResetMap, SpawnIndex);
}

void UEntityEventSubsystem::Deinitialize()
{
	OnRespawnRequest.Clear();

	UCollectionUtils::ClearMapWithMulticastDelegates(OnRespawnCompleteMap);
	UCollectionUtils::ClearMapWithMulticastDelegates(OnRoundStartMap);
	UCollectionUtils::ClearMapWithMulticastDelegates(OnRewardCollectedMap);
	UCollectionUtils::ClearMapWithMulticastDelegates(OnRoundEndMap);
	
	Super::Deinitialize();
}
