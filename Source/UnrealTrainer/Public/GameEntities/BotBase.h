// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameCore/Interfaces/GameEntityInterface.h"
#include "GameFramework/Pawn.h"
#include "GameShared/Components/RawMovementComponent.h"
#include "BotBase.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class UNREALTRAINER_API ABotBase : public APawn, public IGameEntityInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URawMovementComponent* RawMovementComponent;
	
	ABotBase();
	
	virtual TEnumAsByte<EEntityTypes> GetEntityType() const override;
protected:
	UFUNCTION()
	void OnSphereComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& HitResult);
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
