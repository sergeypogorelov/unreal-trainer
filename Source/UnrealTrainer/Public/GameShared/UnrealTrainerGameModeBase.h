// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataAssets/GamePlaySettings.h"
#include "GameCore/DataAssets/TrainingSettings.h"
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

	virtual void BeginPlay() override;
private:
	bool SpawnTrainingAreas();
};
