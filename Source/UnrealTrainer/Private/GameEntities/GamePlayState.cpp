// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/GamePlayState.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
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

void AGamePlayState::BeginPlay()
{
	Super::BeginPlay();
	
	UEntityRegistrySubsystem* RegistrySubsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	RegistrySubsystem->RegisterEntity(this);
	
	UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnRewardCollected.AddUObject(this, &AGamePlayState::OnRewardCollected);
	EventSubsystem->OnRespawnComplete.AddUObject(this, &AGamePlayState::StartRound);
	EventSubsystem->OnGameModeBeginPlay.AddUObject(this, &AGamePlayState::OnGameModeBeginPlay);
}

void AGamePlayState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);

	UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnGameModeBeginPlay.RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}

void AGamePlayState::OnGameModeBeginPlay()
{
	const UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnRespawnRequest.Broadcast();
}

void AGamePlayState::ResetRoundState()
{
	ClearStepTimer();
	
	bIsRoundStarted = false;
	bIsRoundStopped = false;
	StepsCompleted = 0;
	RewardsCollected = 0;
}

void AGamePlayState::StartRound()
{
	UPrintUtils::PrintAsWarning(TEXT("StartRound"));
	
	if (bIsRoundStarted)
	{
		UPrintUtils::PrintAsWarning(TEXT("Attempt to start round when it is alredy started"));
		return;
	}

	bIsRoundStarted = true;

	const UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnRoundStart.Broadcast();
	
	SetStepTimer();
}

void AGamePlayState::SetStepTimer()
{
	ClearStepTimer();
	
	const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
	const float Rate = ConfigSubsystem->GamePlaySettingsPtr->DurationOfOneStep;
	
	GetWorld()->GetTimerManager().SetTimer(StepTimerHandle, this, &AGamePlayState::OnStepTimerComplete, Rate);

	const UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnStepStart.Broadcast();
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
	UPrintUtils::PrintAsWarning(TEXT("OnStepTimerComplete"));
	
	++StepsCompleted;
	
	const UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnStepEnd.Broadcast();

	const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
	if (StepsCompleted == ConfigSubsystem->GamePlaySettingsPtr->CountOfSteps)
	{
		StopRound(false);
	}
	else
	{
		/// TODO: remove once trainer is implemented
		SetStepTimer();
	}
}

void AGamePlayState::OnRewardCollected()
{
	UPrintUtils::PrintAsWarning(TEXT("OnRewardCollected"));
	
	++RewardsCollected;

	const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
	if (RewardsCollected == ConfigSubsystem->GamePlaySettingsPtr->CountOfRewardsToSpawn)
	{
		StopRound(true);
	}
}

void AGamePlayState::StopRound(const bool bIsVictorious)
{
	UPrintUtils::PrintAsWarning(TEXT("StopRound"));
	
	if (bIsRoundStopped)
	{
		UPrintUtils::PrintAsWarning(TEXT("Attempt to stop round when it is alredy stopped"));
		return;
	}

	bIsRoundStopped = true;
	
	ClearStepTimer();

	const UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnRoundEnd.Broadcast(bIsVictorious);

	/// TODO: remove once trainer is implemented
	ResetRoundState();
	EventSubsystem->OnRespawnRequest.Broadcast();
}
