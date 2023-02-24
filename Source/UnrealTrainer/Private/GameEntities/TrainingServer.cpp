// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/TrainingServer.h"
#include "JsonObjectConverter.h"
#include "SIOJLibrary.h"
#include "GameCore/Structs/TrainingAction.h"
#include "GameCore/Structs/TrainingLaunch.h"
#include "GameCore/Structs/TrainingReset.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
#include "GameShared/Subsystems/EntityEventSubsystem.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"
#include "GameShared/Subsystems/GlobalEventSubsystem.h"
#include "GameShared/Utils/PrintUtils.h"

ATrainingServer::ATrainingServer()
{
	PrimaryActorTick.bCanEverTick = true;

	SocketClientComponent = CreateDefaultSubobject<USocketIOClientComponent>(TEXT("SocketIOClient"));
	SocketClientComponent->bShouldAutoConnect = false;
	SocketClientComponent->OnConnected.AddDynamic(this, &ATrainingServer::OnServerConnected);
	SocketClientComponent->OnConnectionProblems.AddDynamic(this, &ATrainingServer::OnServerConnectionError);
}

TEnumAsByte<EEntityTypes> ATrainingServer::GetEntityType() const
{
	return Server;
}

void ATrainingServer::SendObservations(const FTrainingObservations& Observation)
{
	SocketClientComponent->EmitNative(TEXT("sendobservation"), FTrainingObservations::StaticStruct(), &Observation);
}

void ATrainingServer::OnServerConnected(FString SocketId, FString SessionId, bool bIsReconnection)
{
	UPrintUtils::PrintAsInfo(TEXT("Connected"));

	FTrainingLaunch Launch;
	Launch.EnvCount = 1;
	SocketClientComponent->EmitNative(TEXT("launchmodel"), FTrainingLaunch::StaticStruct(), &Launch);

	UPrintUtils::PrintAsInfo(TEXT("Launched"));
}

void ATrainingServer::OnServerConnectionError(int32 Attempts, int32 NextAttemptInMs, float TimeSinceConnected)
{
	UPrintUtils::PrintAsError(TEXT("Connection problems"));
}

void ATrainingServer::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->RegisterEntity(this);

	UGlobalEventSubsystem* GlobalEventSubsystem = GetGameInstance()->GetSubsystem<UGlobalEventSubsystem>();
	UEntityEventSubsystem* EntityEventSubsystem = GetGameInstance()->GetSubsystem<UEntityEventSubsystem>();
	
	GlobalEventSubsystem->OnStaticEntitiesSpawned.AddLambda([this, EntityEventSubsystem]()
	{
		SocketClientComponent->OnNativeEvent(TEXT("reset"), [EntityEventSubsystem](const FString& EventName, const TSharedPtr<FJsonValue>& JsonValuePtr)
		{
			FTrainingReset TrainingReset;
			USIOJConvert::JsonObjectToUStruct(JsonValuePtr->AsObject(), FTrainingReset::StaticStruct(), &TrainingReset);
			EntityEventSubsystem->OnTrainingReset(TrainingReset.EnvId).Broadcast();
		});
		
		SocketClientComponent->OnNativeEvent(TEXT("sendaction"), [EntityEventSubsystem](const FString& EventName, const TSharedPtr<FJsonValue>& JsonValuePtr)
		{
			FTrainingAction TrainingAction;
			USIOJConvert::JsonObjectToUStruct(JsonValuePtr->AsObject(), FTrainingAction::StaticStruct(), &TrainingAction);
			EntityEventSubsystem->OnTrainingActionReceived(TrainingAction.EnvId).Broadcast(TrainingAction);
		});
		
		const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
		SocketClientComponent->Connect(ConfigSubsystem->TrainingSettingsPtr->ServerUrl);
	});
}

void ATrainingServer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);

	if (SocketClientComponent->bIsConnected)
	{
		SocketClientComponent->UnbindEvent(TEXT("reset"));
		SocketClientComponent->UnbindEvent(TEXT("sendaction"));
		SocketClientComponent->Disconnect();
	}
	
	Super::EndPlay(EndPlayReason);
}
