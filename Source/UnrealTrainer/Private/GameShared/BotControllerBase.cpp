// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/BotControllerBase.h"

ABotControllerBase::ABotControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABotControllerBase::BeginPlay()
{
	Super::BeginPlay();
}
