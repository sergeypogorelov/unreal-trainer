// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntities/BotBase.h"
#include "GameCore/Interfaces/GameRewardInterface.h"
#include "GameShared/Subsystems/EntityRegistrySubsystem.h"

ABotBase::ABotBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.f);
	SphereComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = SphereComponent;
	
	RawMovementComponent = CreateDefaultSubobject<URawMovementComponent>(TEXT("RawMovementComponent"));
}

TEnumAsByte<EEntityTypes> ABotBase::GetEntityType() const
{
	return Bot;
}

int32 ABotBase::GetSpawnIndex() const
{
	return SpawnIndex;
}

void ABotBase::SetSpawnIndex(const int32 SpawnIndexVar)
{
	SpawnIndex = SpawnIndexVar;
}

void ABotBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AddMovementInput(MovementDirection, MovementScale);
}

void ABotBase::OnSphereComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                    FVector Normal, const FHitResult& HitResult)
{
	IGameRewardInterface* Reward = Cast<IGameRewardInterface>(OtherActor);
	if (Reward)
	{
		Reward->CollectReward();
	}
}

void ABotBase::BeginPlay()
{
	Super::BeginPlay();

	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->RegisterEntity(this);

	SphereComponent->OnComponentHit.AddDynamic(this, &ABotBase::OnSphereComponentHit);
}

void ABotBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UEntityRegistrySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UEntityRegistrySubsystem>();
	Subsystem->UnregisterEntity(this);
	
	Super::EndPlay(EndPlayReason);
}
