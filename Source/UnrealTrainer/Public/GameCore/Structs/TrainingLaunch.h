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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Algselected = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Trainepisodes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Evalepisodes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Conactionspace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Disactionspace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Actionspace = TEXT("");;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Observationspace = TEXT("");;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Loadmodel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Savemodel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Modelname = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Layers = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Lr = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplorationRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplorationRateDecay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplorationFinal = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HistorySize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpdateGameFrequncy = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpdateSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RewardDiscount = 0.f;
};
