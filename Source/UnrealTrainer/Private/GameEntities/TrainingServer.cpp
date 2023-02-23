// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/TrainingServer.h"
#include "JsonObjectConverter.h"
#include "SIOJLibrary.h"
#include "GameCore/Structs/TrainingLaunch.h"
#include "GameShared/Subsystems/ConfigRegistrySubsystem.h"
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

	UGlobalEventSubsystem* EventSubsystem = GetGameInstance()->GetSubsystem<UGlobalEventSubsystem>();
	EventSubsystem->OnStaticEntitiesSpawned.AddLambda([this]()
	{
		const UConfigRegistrySubsystem* ConfigSubsystem = GetGameInstance()->GetSubsystem<UConfigRegistrySubsystem>();
		SocketClientComponent->Connect(ConfigSubsystem->TrainingSettingsPtr->ServerUrl);
	});
}

void ATrainingServer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}
