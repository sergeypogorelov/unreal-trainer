// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/PrintUtils.h"

void UPrintUtils::PrintAsError(const FString& Message, const float TimeToDisplay)
{
	UE_LOG(LogTemp, Error, TEXT("PrintAsError - \"%s\""), *Message);
	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, TimeToDisplay, FColor::Red, Message);
}

void UPrintUtils::PrintAsWarning(const FString& Message, const float TimeToDisplay)
{
	UE_LOG(LogTemp, Warning, TEXT("PrintAsWarning - \"%s\""), *Message);
	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, TimeToDisplay, FColor::Orange, Message);
}

void UPrintUtils::PrintAsInfo(const FString& Message, const float TimeToDisplay)
{
	UE_LOG(LogTemp, Display, TEXT("PrintAsInfo - \"%s\""), *Message);
	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, TimeToDisplay, FColor::Cyan, Message);
}
