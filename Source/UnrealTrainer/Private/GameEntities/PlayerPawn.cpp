// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/PlayerPawn.h"
#include "Components/SphereComponent.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

TEnumAsByte<EEntityTypes> APlayerPawn::GetEntityType() const
{
	return Player;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->RegisterEntity(this);
	
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	GetMeshComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void APlayerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}
