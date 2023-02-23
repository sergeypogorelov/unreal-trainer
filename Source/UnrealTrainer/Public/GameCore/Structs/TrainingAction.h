// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrainingAction.generated.h"

USTRUCT(BlueprintType)
struct UNREALTRAINER_API FTrainingAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnvId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Action;
};
