﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityEventSubsystem.generated.h"

UCLASS()
class UNREALTRAINER_API UEntityEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TMulticastDelegate<void()> OnGameModeBeginPlay;
	TMulticastDelegate<void(const int32 SpawnIndex)> OnRespawnRequest;
	TMulticastDelegate<void(const int32 SpawnIndex)> OnRespawnComplete;
	TMulticastDelegate<void(const int32 SpawnIndex)> OnRoundStart;
	TMulticastDelegate<void(const int32 SpawnIndex)> OnStepStart;
	TMulticastDelegate<void(const int32 SpawnIndex)> OnStepEnd;
	TMulticastDelegate<void(const int32 SpawnIndex)> OnRewardCollected;
	TMulticastDelegate<void(const int32 SpawnIndex, const bool bIsVictorious)> OnRoundEnd;
	
	virtual void Deinitialize() override;
};
