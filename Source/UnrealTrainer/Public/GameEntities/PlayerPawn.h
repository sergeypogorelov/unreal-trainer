// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameFramework/DefaultPawn.h"
#include "PlayerPawn.generated.h"

UCLASS(BlueprintType)
class UNREALTRAINER_API APlayerPawn : public ADefaultPawn, public IGameEntityInterface
{
	GENERATED_BODY()

public:
	APlayerPawn();

	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
