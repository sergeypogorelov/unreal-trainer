// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MathUtils.generated.h"

UCLASS()
class UNREALTRAINER_API UMathUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static float CalcAngleBetweenLocations(const FVector2D& Location1, const FVector2D& Location2);
};
