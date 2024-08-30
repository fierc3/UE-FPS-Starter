// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EventHandlerActor.h"
#include "ProblemsomeBpFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FMyDelegate, UPsEvent*, Event);

/**
 * 
 */
UCLASS()
class FPSGAME_API UProblemsomeBpFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "PS_Events")
	static AEventHandlerActor* RegisterEventBusHandler(AActor* Actor, FMyDelegate Delegate);
};