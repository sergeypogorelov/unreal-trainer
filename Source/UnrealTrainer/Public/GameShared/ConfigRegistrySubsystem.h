// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataAssets/GamePlaySettings.h"
#include "GameCore/DataAssets/TrainingSettings.h"
#include "UObject/Object.h"
#include "ConfigRegistrySubsystem.generated.h"

UCLASS()
class UNREALTRAINER_API UConfigRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<UGamePlaySettings> GamePlaySettingsPtr;
	TWeakObjectPtr<UTrainingSettings> TrainingSettingsPtr;
};
