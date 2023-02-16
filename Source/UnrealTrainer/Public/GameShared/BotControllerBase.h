// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotControllerBase.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class UNREALTRAINER_API ABotControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	ABotControllerBase();

protected:
	virtual void BeginPlay() override;
};
