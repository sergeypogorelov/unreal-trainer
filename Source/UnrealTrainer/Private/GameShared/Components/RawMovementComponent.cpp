// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Components/RawMovementComponent.h"

URawMovementComponent::URawMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URawMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (IsFrozen())
	{
		return;
	}
	
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsMoveInputIgnored())
	{
		return;
	}

	const FVector PendingInput = GetPendingInputVector().GetClampedToMaxSize(1.f);;
	Velocity = PendingInput * Speed;
	ConsumeInputVector();

	const FVector Delta = Velocity * DeltaTime;
	if (!Delta.IsNearlyZero())
	{
		const FQuat Rotation = UpdatedComponent->GetComponentQuat();

		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);
	}
	
	UpdateComponentVelocity();
}

bool URawMovementComponent::IsFrozen() const
{
	return bIsFrozen;
}

void URawMovementComponent::Freeze()
{
	if (IsFrozen())
	{
		return;
	}

	bIsFrozen = true;
}

void URawMovementComponent::Unfreeze()
{
	if (IsFrozen())
	{
		bIsFrozen = false;
	}
}
