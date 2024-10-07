// Fill out your copyright notice in the Description page of Project Settings.

// EventHandlerHelper.h
#pragma once

#include "CoreMinimal.h"
#include "EventBusActor.h"
#include "EventHandlerActor.h"
#include "PsEvent.h"

class  EventBusHelper
{
public:
    static AEventHandlerActor* SetupAndRegisterEventHandler(UWorld* World, AActor* Owner, TFunction<void(UPsEvent*)> ReceiveFunc);
};