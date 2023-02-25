// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrainingObservations.generated.h"

USTRUCT(BlueprintType)
struct UNREALTRAINER_API FTrainingObservations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnvId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Reward = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Observations = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Done = false;
};
