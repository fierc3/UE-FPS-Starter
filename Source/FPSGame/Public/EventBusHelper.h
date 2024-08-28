// Fill out your copyright notice in the Description page of Project Settings.

// EventHandlerHelper.h
#pragma once

#include "CoreMinimal.h"
#include "EventBusActor.h"
#include "EventHandlerActor.h"

class FPSGAME_API EventBusHelper
{
public:
    static AEventHandlerActor* SetupAndRegisterEventHandler(UWorld* World, AActor* Owner, TFunction<void()> ReceiveFunc);
};