// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ISOLATION_API FInputHelper
{
public:
	FInputHelper();
	~FInputHelper();

	static const TMap<FString, FString> KeyConversionMap;
};
