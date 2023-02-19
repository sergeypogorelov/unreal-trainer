// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectionUtils.generated.h"

UCLASS()
class UNREALTRAINER_API UCollectionUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <typename KeyType, typename ValType>
	static ValType& GetMapValue(TMap<KeyType, ValType>& Map, const KeyType Key)
	{
		if (!Map.Contains(Key))
		{
			ValType Val;
			Map.Add(Key, Val);
		}

		return Map[Key];
	}
	
	template <typename KeyType, typename DelegateSignature>
	static void ClearMapWithMulticastDelegates(TMap<KeyType, TMulticastDelegate<DelegateSignature>>& Map)
	{
		for (TTuple<KeyType, TMulticastDelegate<DelegateSignature>>& Pair : Map)
		{
			Pair.Value.Clear();
		}

		Map.Empty();
	}
};
