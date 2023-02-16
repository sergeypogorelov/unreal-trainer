// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/Enums/EntityTypes.h"
#include "UObject/Interface.h"
#include "GameEntityInterface.generated.h"

UINTERFACE()
class UGameEntityInterface : public UInterface
{
	GENERATED_BODY()
};

class UNREALTRAINER_API IGameEntityInterface
{
	GENERATED_BODY()

public:
	virtual TEnumAsByte<EEntityTypes> GetEntityType() const;
};
