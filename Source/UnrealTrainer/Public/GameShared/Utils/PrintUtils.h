// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrintUtils.generated.h"

UCLASS()
class UNREALTRAINER_API UPrintUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void PrintAsError(const FString& Message, const float& TimeToDisplay = 2.f);

	UFUNCTION(BlueprintCallable)
	static void PrintAsWarning(const FString& Message, const float& TimeToDisplay = 2.f);

	UFUNCTION(BlueprintCallable)
	static void PrintAsInfo(const FString& Message, const float& TimeToDisplay = 2.f);
};
