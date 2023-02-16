// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/TrainingAreaBase.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"

ATrainingAreaBase::ATrainingAreaBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

TEnumAsByte<EEntityTypes> ATrainingAreaBase::GetEntityType() const
{
	return Area;
}

void ATrainingAreaBase::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->RegisterEntity(this);
}

void ATrainingAreaBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}
