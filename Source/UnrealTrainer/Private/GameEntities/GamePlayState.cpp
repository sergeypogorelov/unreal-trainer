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
	EventSubsystem->OnRespawnComplete.AddLambda([this]()
	{
		StartRound();
	});
	EventSubsystem->OnGameModeBeginPlay.AddLambda([EventSubsystem]()
	{
		EventSubsystem->OnRespawnRequest.Broadcast();
	});
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
}

void AGamePlayState::StartRound()
{
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
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
	{
		OnStepTimerComplete();
	});
	
	const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
	const float Rate = ConfigSubsystem->GamePlaySettingsPtr->DurationOfOneStep;
	
	GetWorld()->GetTimerManager().SetTimer(StepTimerHandle, TimerDelegate, Rate, false);

	const UEntityEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	EventSubsystem->OnStepStart.Broadcast();
}

void AGamePlayState::ClearStepTimer()
{
	StepTimerHandle.Invalidate();
}

void AGamePlayState::OnStepTimerComplete()
{
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

void AGamePlayState::StopRound(const bool bIsVictorious)
{
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
