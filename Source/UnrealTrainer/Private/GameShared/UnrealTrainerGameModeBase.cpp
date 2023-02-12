// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameShared/UnrealTrainerGameModeBase.h"
#include "GameShared/EntityRegistrySubsystem.h"

void AUnrealTrainerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	TArray<AActor*> Actors = Subsystem->GetEntitiesByType(EEntityTypes::Spot);
	UE_LOG(LogTemp, Warning, TEXT("Found Actors:"));
	for (const AActor* FoundActor : Actors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Actor - %s"), *FoundActor->GetFullName());
	}
}
