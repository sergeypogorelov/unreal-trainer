// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameMultiSpawnInterface.generated.h"

UINTERFACE()
class UGameMultiSpawnInterface : public UInterface
{
	GENERATED_BODY()
};

class UNREALTRAINER_API IGameMultiSpawnInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetSpawnIndex() const;
	virtual void SetSpawnIndex(const int32& SpawnIndex);
};
