// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/GamePlayState.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Subsystems/GlobalEventSubsystem.h"
#include "GameShared/Utils/PrintUtils.h"

AGamePlayState::AGamePlayState()
{
	PrimaryActorTick.bCanEverTick = true;

	ResetRoundState();
}

TEnumAsByte<EEntityTypes> AGamePlayState::GetEntityType() const
{
	return State;
}

int32 AGamePlayState::GetSpawnIndex() const
{
	return SpawnIndex;
}

void AGamePlayState::SetSpawnIndex(const int32 SpawnIndexVar)
{
	SpawnIndex = SpawnIndexVar;
}

void AGamePlayState::RequestForRestart()
{
	ResetRoundState();

	const UGlobalEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UGlobalEventSubsystem>();
	EventSubsystem->OnDynamicEntitiesSpawnRequest.Broadcast(GetSpawnIndex());
}

void AGamePlayState::CheckStep()
{
	RewardsCollected = 0;
	
	UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnStepStart(GetSpawnIndex()).Broadcast();
	
	SetStepTimer();
}

void AGamePlayState::BeginPlay()
{
	Super::BeginPlay();
	
	UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	RegistrySubsystem->RegisterEntity(this);

	SetUpEventHandlers();
}

void AGamePlayState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}

void AGamePlayState::ResetRoundState()
{
	ClearStepTimer();
	
	bIsRoundStarted = false;
	bIsRoundStopped = false;
	StepsCompleted = 0;
	RewardsCollected = 0;
	RewardsCollectedInTotal = 0;
}

void AGamePlayState::StartRound()
{
	if (bIsRoundStarted)
	{
		UPrintUtils::PrintAsWarning(TEXT("Attempt to start round when it is alredy started"));
		return;
	}

	bIsRoundStarted = true;

	UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnRoundStart(GetSpawnIndex()).Broadcast();
}

void AGamePlayState::SetStepTimer()
{
	ClearStepTimer();
	
	const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
	const float Rate = ConfigSubsystem->GamePlaySettingsPtr->DurationOfOneStep;
	
	GetWorld()->GetTimerManager().SetTimer(StepTimerHandle, this, &AGamePlayState::OnStepTimerComplete, Rate);
}

void AGamePlayState::ClearStepTimer()
{
	if (!StepTimerHandle.IsValid())
	{
		return;
	}
	
	if (GetWorld()->GetTimerManager().TimerExists(StepTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(StepTimerHandle);
	}
}

void AGamePlayState::OnStepTimerComplete()
{
	++StepsCompleted;

	const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
	if (StepsCompleted == ConfigSubsystem->GamePlaySettingsPtr->CountOfSteps)
	{
		StopRound(false);
	}
	
	UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnStepEnd(GetSpawnIndex()).Broadcast(RewardsCollected);
}

void AGamePlayState::StopRound(const bool bIsVictorious)
{
	if (bIsRoundStopped)
	{
		UPrintUtils::PrintAsWarning(TEXT("Attempt to stop round when it is alredy stopped"));
		return;
	}

	bIsRoundStopped = true;
	
	ClearStepTimer();

	UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnRoundEnd(GetSpawnIndex()).Broadcast(bIsVictorious, RewardsCollected);
}

void AGamePlayState::SetUpEventHandlers()
{
	UGlobalEventSubsystem* GlobalEventSubsystem = GetGameInstance()->GetSubsystem<UGlobalEventSubsystem>();
	GlobalEventSubsystem->OnStaticEntitiesSpawned.AddLambda([this, GlobalEventSubsystem]()
	{
		UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
		
		EventSubsystem->OnRewardCollected(GetSpawnIndex()).AddLambda([this]()
		{
			++RewardsCollected;
			++RewardsCollectedInTotal;

			const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
			if (RewardsCollectedInTotal == ConfigSubsystem->GamePlaySettingsPtr->CountOfRewardsToSpawn)
			{
				StopRound(true);
			}
		});
		
		GlobalEventSubsystem->OnDynamicEntitiesSpawned(GetSpawnIndex()).AddLambda([this]()
		{
			StartRound();
		});
		
		GlobalEventSubsystem->OnDynamicEntitiesSpawnRequest.Broadcast(GetSpawnIndex());
	});
}
