// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataAssets/GamePlaySettings.h"
#include "GameCore/DataAssets/TrainingSettings.h"
#include "GameEntities/BotBase.h"
#include "GameEntities/RewardBase.h"
#include "GameEntities/TrainingAreaBase.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealTrainerGameModeBase.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class UNREALTRAINER_API AUnrealTrainerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGamePlaySettings* GamePlaySettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTrainingSettings* TrainingSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATrainingAreaBase> TrainingAreaClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABotBase> BotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ARewardBase> RewardClass;

	virtual void BeginPlay() override;
private:
	float OriginTrainingAreaLengthOnX = 0.f;
	float OriginTrainingAreaLengthOnY = 0.f;
	FTransform OriginTrainingAreaTransform;
	TWeakObjectPtr<ATrainingAreaBase> OriginTrainingAreaPtr;

	TArray<FTransform> OriginSpotTransforms;

	bool TryInitConfigs();
	bool TryInitOriginTrainingArea();
	bool TryInitOriginSpots();
	bool TrySpawnTrainingAreas();
	bool TrySpawnStaticEntities();
	bool TrySpawnDynamicEntities(const int32 SpawnIndex);
	void SetUpEventHandlers();
	void ChangeTransformBySpawnIndex(FTransform& NewTransform, const FTransform& OriginTransform, const int32 SpawnIndex);
};
