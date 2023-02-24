// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/Trainer.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Subsystems/GlobalEventSubsystem.h"
#include "GameShared/Utils/PrintUtils.h"
#include "Kismet/KismetSystemLibrary.h"

ATrainer::ATrainer()
{
	PrimaryActorTick.bCanEverTick = true;
}

TEnumAsByte<EEntityTypes> ATrainer::GetEntityType() const
{
	return Trainer;
}

int32 ATrainer::GetSpawnIndex() const
{
	return SpawnIndex;
}

void ATrainer::SetSpawnIndex(const int32 SpawnIndexVar)
{
	SpawnIndex = SpawnIndexVar;
}

void ATrainer::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	RegistrySubsystem->RegisterEntity(this);
	
	UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	UGlobalEventSubsystem* GlobalEventSubsystem = GetGameInstance()->GetSubsystem<UGlobalEventSubsystem>();
	
	GlobalEventSubsystem->OnStaticEntitiesSpawned.AddLambda([this, RegistrySubsystem, EntityEventSubsystem, GlobalEventSubsystem]()
	{
		TrainingServerPtr = Cast<ATrainingServer>(RegistrySubsystem->GetTrainingServer());
		GamePlayState = Cast<AGamePlayState>(RegistrySubsystem->GetGamePlayState(GetSpawnIndex()));

		GlobalEventSubsystem->OnDynamicEntitiesSpawned(GetSpawnIndex()).AddLambda([this, RegistrySubsystem]()
		{
			BotPtr = Cast<ABotBase>(RegistrySubsystem->GetBot(GetSpawnIndex()));
			BotControllerPtr = Cast<ABotControllerBase>(BotPtr->GetController());

			UPrintUtils::PrintAsInfo(TEXT("OnRespawnComplete"));
		});
		EntityEventSubsystem->OnTrainingActionReceived(GetSpawnIndex()).AddLambda([this](const FTrainingAction& Action)
		{
			UPrintUtils::PrintAsInfo(TEXT("OnTrainingActionReceived"));
		});
		EntityEventSubsystem->OnTrainingReset(GetSpawnIndex()).AddLambda([this]()
		{
			UPrintUtils::PrintAsInfo(TEXT("OnTrainingReset"));
		});
	});
}

void ATrainer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}
