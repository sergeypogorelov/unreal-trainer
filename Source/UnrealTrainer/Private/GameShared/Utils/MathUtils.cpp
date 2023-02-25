// Fill out your copyright notice in the Description page of Project Settings.

#include "GameShared/Utils/MathUtils.h"
#include "Kismet/KismetMathLibrary.h"

float UMathUtils::CalcAngleBetweenLocations(const FVector2D& Location1, const FVector2D& Location2)
{
	const float Angle = UKismetMathLibrary::Atan2(Location2.Y - Location1.Y, Location2.X - Location1.X);
	const float Degree = fmod(Angle * (180.f / UKismetMathLibrary::GetPI()) + 360.f, 360.f);

	return Degree;
}
