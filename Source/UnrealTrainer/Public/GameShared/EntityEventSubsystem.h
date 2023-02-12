// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityEventSubsystem.generated.h"

UCLASS()
class UNREALTRAINER_API UEntityEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TMulticastDelegate<void()> OnGameModeBeginPlay;
};
