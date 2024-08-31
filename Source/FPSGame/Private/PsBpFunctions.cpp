// Fill out your copyright notice in the Description page of Project Settings.


#include "PsBpFunctions.h"
#include "EventBusHelper.h"
#include "EventHandlerActor.h"

AEventHandlerActor* UPsBpFunctions::RegisterEventBusHandler(AActor* Actor, FMyDelegate Delegate)
{
    TFunction<void(UPsEvent* Event)> fp = [Delegate](UPsEvent* Event) { Delegate.ExecuteIfBound(Event); };
    return EventBusHelper::SetupAndRegisterEventHandler(Actor->GetWorld(), Actor, fp);
}