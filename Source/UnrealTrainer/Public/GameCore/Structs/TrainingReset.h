// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrainingReset.generated.h"

USTRUCT(BlueprintType)
struct UNREALTRAINER_API FTrainingReset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnvId;
};
