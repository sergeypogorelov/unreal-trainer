// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GlobalEventSubsystem.generated.h"

UCLASS()
class UNREALTRAINER_API UGlobalEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TMulticastDelegate<void()> OnConfigsInitialized;
	TMulticastDelegate<void()> OnStaticEntitiesSpawned;
};
