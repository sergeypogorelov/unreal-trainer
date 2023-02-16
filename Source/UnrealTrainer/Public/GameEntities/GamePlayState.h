// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameFramework/Actor.h"
#include "GamePlayState.generated.h"

UCLASS()
class UNREALTRAINER_API AGamePlayState : public AActor, public IGameEntityInterface
{
	GENERATED_BODY()

public:
	AGamePlayState();

	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	bool bIsRoundStarted;
	bool bIsRoundStopped;
	int32 StepsCompleted;
	int32 RewardsCollected;
	FTimerHandle StepTimerHandle;

	void ResetRoundState();
	void StartRound();
	void SetStepTimer();
	void ClearStepTimer();
	void OnStepTimerComplete();
	void StopRound(const bool bIsVictorious);
	void SetUpEventHandlers();
};
