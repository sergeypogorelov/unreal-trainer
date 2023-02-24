// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "RawMovementComponent.generated.h"

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class UNREALTRAINER_API URawMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(ClampMin="0", ToolTip="Speed of movement in cm/s"))
	float Speed = 100.f;
	
	URawMovementComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsFrozen() const;
	void Freeze();
	void Unfreeze();
private:
	bool bIsFrozen;
};
