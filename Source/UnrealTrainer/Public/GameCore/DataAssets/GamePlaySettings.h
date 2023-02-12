// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlaySettings.generated.h"

UCLASS(BlueprintType)
class UNREALTRAINER_API UGamePlaySettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldRandomize = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DurationOfOneStep = .2f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CountOfSteps = 25;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CountOfRewardsToSpawn = 1;
};
