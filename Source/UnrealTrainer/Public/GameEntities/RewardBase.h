// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameCore/Interfaces/GameMultiSpawnInterface.h"
#include "GameCore/Interfaces/GameRewardInterface.h"
#include "GameFramework/Actor.h"
#include "RewardBase.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class UNREALTRAINER_API ARewardBase : public AActor, public IGameEntityInterface, public IGameRewardInterface, public IGameMultiSpawnInterface
{
	GENERATED_BODY()

public:
	ARewardBase();
	
	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
	virtual int32 GetSpawnIndex() const override;
	virtual void SetSpawnIndex(const int32& SpawnIndexVar) override;
	virtual void CollectReward() override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	int32 SpawnIndex;
};
