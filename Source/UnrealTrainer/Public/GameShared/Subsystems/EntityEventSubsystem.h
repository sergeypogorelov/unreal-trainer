// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Structs/TrainingAction.h"
#include "EntityEventSubsystem.generated.h"

UCLASS()
class UNREALTRAINER_API UEntityEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TMulticastDelegate<void()>& OnRoundStart(const int32 SpawnIndex);
	TMulticastDelegate<void()>& OnStepStart(const int32 SpawnIndex);
	TMulticastDelegate<void()>& OnStepEnd(const int32 SpawnIndex);
	TMulticastDelegate<void()>& OnRewardCollected(const int32 SpawnIndex);
	TMulticastDelegate<void(const bool bIsVictorious)>& OnRoundEnd(const int32 SpawnIndex);
	TMulticastDelegate<void(const FTrainingAction& Action)>& OnTrainingActionReceived(const int32 SpawnIndex);
	TMulticastDelegate<void()>& OnTrainingReset(const int32 SpawnIndex);
	
	virtual void Deinitialize() override;
private:
	TMap<int32, TMulticastDelegate<void()>> OnRoundStartMap;
	TMap<int32, TMulticastDelegate<void()>> OnStepStartMap;
	TMap<int32, TMulticastDelegate<void()>> OnStepEndMap;
	TMap<int32, TMulticastDelegate<void()>> OnRewardCollectedMap;
	TMap<int32, TMulticastDelegate<void(const bool bIsVictorious)>> OnRoundEndMap;
	TMap<int32, TMulticastDelegate<void(const FTrainingAction& Action)>> OnTrainingActionReceivedMap;
	TMap<int32, TMulticastDelegate<void()>> OnTrainingResetMap;
};
