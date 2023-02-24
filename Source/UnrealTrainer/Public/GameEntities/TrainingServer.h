// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SocketIOClientComponent.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameCore/Structs/TrainingObservations.h"
#include "GameFramework/Actor.h"
#include "TrainingServer.generated.h"

UCLASS()
class UNREALTRAINER_API ATrainingServer : public AActor, public IGameEntityInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USocketIOClientComponent* SocketClientComponent;
	
	ATrainingServer();

	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
	void SendObservations(const FTrainingObservations& Observation);
protected:
	UFUNCTION()
	void OnServerConnected(FString SocketId, FString SessionId, bool bIsReconnection);

	UFUNCTION()
	void OnServerConnectionError(int32 Attempts, int32  NextAttemptInMs, float TimeSinceConnected);
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
