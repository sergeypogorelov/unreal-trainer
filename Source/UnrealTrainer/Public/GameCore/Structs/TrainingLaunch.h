// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrainingLaunch.generated.h"

USTRUCT(BlueprintType)
struct UNREALTRAINER_API FTrainingLaunch
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnvCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Algselected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Trainepisodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Evalepisodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Conactionspace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Disactionspace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Actionspace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Observationspace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Loadmodel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Savemodel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Modelname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Layers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Lr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplorationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplorationRateDecay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplorationFinal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HistorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpdateGameFrequncy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpdateSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RewardDiscount;
};
