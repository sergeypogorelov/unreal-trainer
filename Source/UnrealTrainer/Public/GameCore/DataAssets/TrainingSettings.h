// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrainingSettings.generated.h"

UCLASS(BlueprintType)
class UNREALTRAINER_API UTrainingSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CountOfAgents = 1;
};
