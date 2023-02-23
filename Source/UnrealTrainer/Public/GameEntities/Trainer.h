// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BotBase.h"
#include "TrainingServer.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameCore/Interfaces/GameMultiSpawnInterface.h"
#include "GameFramework/Actor.h"
#include "GameShared/BotControllerBase.h"
#include "Trainer.generated.h"

UCLASS()
class UNREALTRAINER_API ATrainer : public AActor, public IGameEntityInterface, public IGameMultiSpawnInterface
{
	GENERATED_BODY()

public:
	ATrainer();

	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
	virtual int32 GetSpawnIndex() const override;
	virtual void SetSpawnIndex(const int32 SpawnIndexVar) override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	int32 SpawnIndex;
	
	TWeakObjectPtr<ATrainingServer> TrainingServerPtr;
	TWeakObjectPtr<ABotBase> BotPtr;
	TWeakObjectPtr<ABotControllerBase> BotControllerPtr;
};
