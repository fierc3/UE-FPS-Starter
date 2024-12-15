// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EventHandlerActor.h"
#include "PsBpFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FMyDelegate, UPsEvent*, Event);
DECLARE_DYNAMIC_DELEGATE(FStartTimerCompleteDelegate);

/**
 * 
 */
UCLASS()
class  UPsBpFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "PS_Events")
	static AEventHandlerActor* RegisterEventBusHandler(AActor* Actor, FMyDelegate Delegate);
	UFUNCTION(BlueprintCallable, Category = "PS_Events")
	static UPsEvent* CreatePsEvent(EEventType InEventType, float InValue, AActor* InOrigin, AActor* InTarget, FName InAdditionalData);
	UFUNCTION(BlueprintCallable, Category = "PS_Timer")
	static void StartTimer(UObject* WorldContextObject, float Duration, FStartTimerCompleteDelegate InTimerCompleteDelegate);
};