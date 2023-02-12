// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/SpotBase.h"

#include "GameShared/EntityEventSubsystem.h"
#include "GameShared/EntityRegistrySubsystem.h"
#include "GameShared/PrintUtils.h"

ASpotBase::ASpotBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

TEnumAsByte<EEntityTypes> ASpotBase::GetEntityType() const
{
	return Spot;
}

void ASpotBase::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->RegisterEntity(this);

	UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EntityEventSubsystem->OnGameModeBeginPlay.AddLambda([]()
	{
		UPrintUtils::PrintAsInfo(TEXT("Hello from Spot"));
	});
}

void ASpotBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}
