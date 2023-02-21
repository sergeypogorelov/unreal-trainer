// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrainingLaunch.generated.h"

USTRUCT(BlueprintType)
struct UNREALTRAINER_API FTrainingLaunch
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnvCount = 1;
};
