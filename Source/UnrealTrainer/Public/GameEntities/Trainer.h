// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BotBase.h"
#include "GamePlayState.h"
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
	inline const static TMap<int32, FVector> ActionToDirectionMap {
		{ 0, FVector(0, 0, 0) },
		{ 1, FVector(0, -1, 0) },
		{ 2, FVector(1, 0, 0) },
		{ 3, FVector(0, 1, 0) },
		{ 4, FVector(-1, 0, 0) },
	};
	
	ATrainer();

	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
	virtual int32 GetSpawnIndex() const override;
	virtual void SetSpawnIndex(const int32 SpawnIndexVar) override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	bool bIsServerReady;
	int32 SpawnIndex;
	TArray<float> Observations;
	
	TWeakObjectPtr<ATrainingServer> TrainingServerPtr;
	TWeakObjectPtr<AGamePlayState> GamePlayStatePtr;
	TWeakObjectPtr<ABotBase> BotPtr;
	TWeakObjectPtr<ABotControllerBase> BotControllerPtr;

	void SendObservations(const int32 Rewards, const bool bIsDone);
};
