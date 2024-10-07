// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class  LogHelper
{
public:
	LogHelper();
	~LogHelper();
	static void PrintLog(const FString& Message, const FColor* Color = nullptr, float Duration = 15.0f);
private:
	static FColor GetColorForMessage(const FString& Message);
};
