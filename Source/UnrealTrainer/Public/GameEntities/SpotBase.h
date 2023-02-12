// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "SpotBase.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class UNREALTRAINER_API ASpotBase : public ATargetPoint, public IGameEntityInterface
{
	GENERATED_BODY()

public:
	ASpotBase();

	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
