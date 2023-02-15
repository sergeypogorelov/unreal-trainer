// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/RewardBase.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"

ARewardBase::ARewardBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

TEnumAsByte<EEntityTypes> ARewardBase::GetEntityType() const
{
	return Reward;
}

void ARewardBase::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->RegisterEntity(this);
}

void ARewardBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}
