// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrainingObservations.generated.h"

USTRUCT(BlueprintType)
struct UNREALTRAINER_API FTrainingObservations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnvId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Reward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Observations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Done;
};
