// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityTypes.generated.h"

UENUM(BlueprintType)
enum EEntityTypes
{
	Undefined,
	Bot,
	Reward,
	Spot,
	Area,
	Trainer
};
