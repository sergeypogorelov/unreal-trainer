// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Utils/CollectionUtils.h"

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRoundStart(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnRoundStartMap, SpawnIndex);
}

TMulticastDelegate<void()>& UEntityEventSubsystem::OnStepStart(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnStepStartMap, SpawnIndex);
}

TMulticastDelegate<void(const int32 RewardsCollected)>& UEntityEventSubsystem::OnStepEnd(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnStepEndMap, SpawnIndex);
}

TMulticastDelegate<void()>& UEntityEventSubsystem::OnRewardCollected(const int32 SpawnIndex)
{
	return UCollectionUtils::GetMapValue(OnRewardCollectedMap, SpawnIndex);
}

TMulticastDelegate<void(const bool bIsVictorious, const int32 RewardsCollected)>& UEntityEventSubsystem::OnRoundEnd(const int32 SpawnIndex)
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
	UCollectionUtils::ClearMapWithMulticastDelegates(OnRoundStartMap);
	UCollectionUtils::ClearMapWithMulticastDelegates(OnRewardCollectedMap);
	UCollectionUtils::ClearMapWithMulticastDelegates(OnRoundEndMap);
	
	Super::Deinitialize();
}
