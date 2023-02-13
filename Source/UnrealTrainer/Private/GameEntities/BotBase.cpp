// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/BotBase.h"
#include "GameShared/EntityRegistrySubsystem.h"

ABotBase::ABotBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RawMovementComponent = CreateDefaultSubobject<URawMovementComponent>(TEXT("RawMovementComponent"));
}

TEnumAsByte<EEntityTypes> ABotBase::GetEntityType() const
{
	return Bot;
}

void ABotBase::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->RegisterEntity(this);
}

void ABotBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}
