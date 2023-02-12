// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataAssets/GamePlaySettings.h"
#include "GameCore/DataAssets/TrainingSettings.h"
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

	virtual void BeginPlay() override;
};
