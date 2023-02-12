// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCore/Interfaces/GameEntityInterface.h"

TEnumAsByte<EEntityTypes> IGameEntityInterface::GetEntityType() const
{
	return Undefined;
}
