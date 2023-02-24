// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameCore/Interfaces/GameMultiSpawnInterface.h"
#include "GameFramework/Actor.h"
#include "GamePlayState.generated.h"

UCLASS()
class UNREALTRAINER_API AGamePlayState : public AActor, public IGameEntityInterface, public IGameMultiSpawnInterface
{
	GENERATED_BODY()

public:
	AGamePlayState();

	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
	virtual int32 GetSpawnIndex() const override;
	virtual void SetSpawnIndex(const int32 SpawnIndexVar) override;
	void RequestForRestart();
	void CheckForStep();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	bool bIsRoundStarted;
	bool bIsRoundStopped;
	int32 StepsCompleted;
	int32 RewardsCollected;
	int32 SpawnIndex;
	FTimerHandle StepTimerHandle;

	void ResetRoundState();
	void StartRound();
	void SetStepTimer();
	void ClearStepTimer();
	void OnStepTimerComplete();
	void StopRound(const bool bIsVictorious);
	void SetUpEventHandlers();
};
