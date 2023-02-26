// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrainingSettings.generated.h"

UCLASS(BlueprintType)
class UNREALTRAINER_API UTrainingSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Common")
	bool bShouldEvaluate = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Common")
	bool bShouldNormalize = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Common")
	int32 CountOfAgents = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Common")
	float AreasOffset = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Common")
	FString ServerUrl = TEXT("ws://localhost:8017/socket.io/");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Common")
	FString ModelName = TEXT("test_model");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	int32 TrainEpisodes = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	int32 EvalEpisodes = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	FString Layer = TEXT("[64, 64]");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	float Lr = .0001f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	float ExplorationRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	float ExplorationRateDecay = .9998f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	float ExplorationFinal = .05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	int32 HistorySize = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	int32 UpdateGameFrequency = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	int32 UpdateSize = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Params")
	float RewardDiscount = .99f;
};
