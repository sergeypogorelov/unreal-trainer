// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameFramework/Actor.h"
#include "TrainingAreaBase.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class UNREALTRAINER_API ATrainingAreaBase : public AActor, public IGameEntityInterface
{
	GENERATED_BODY()

public:
	ATrainingAreaBase();

	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float GetLengthOnX() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float GetLengthOnY() const;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
