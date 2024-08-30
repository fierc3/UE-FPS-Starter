// Fill out your copyright notice in the Description page of Project Settings.


#include "EventBusHelper.h"
#include "LogHelper.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "PsEvent.h"

AEventHandlerActor* EventBusHelper::SetupAndRegisterEventHandler(UWorld* World, AActor* Owner, TFunction<void(UPsEvent*)> ReceiveFunc)
{
    LogHelper::PrintLog(TEXT("Registering EventHandler"));
    AEventHandlerActor* EventHandler = nullptr;
    if (World)
    {
        AEventBusActor* BusInstance = AEventBusActor::GetInstance(World);
        if (BusInstance)
        {
            // Spawn the EventHandler actor
            FActorSpawnParameters SpawnParams;
            EventHandler = World->SpawnActor<AEventHandlerActor>(AEventHandlerActor::StaticClass(), SpawnParams);
            if (EventHandler)
            {
                // Set the Bus pointer after spawning
                EventHandler->SetBus(BusInstance);

                // Register the EventHandler with the BusInstance
                BusInstance->Register(*EventHandler, FEventDelegate::CreateLambda([EventHandler, ReceiveFunc](UPsEvent* Event) {
                    if (Event)
                    {
                        ReceiveFunc(Event);
                    }
                    }));
            }
            else
            {
                LogHelper::PrintLog(TEXT("Failed to spawn EventHandler actor"));
            }
        }
        else
        {
            LogHelper::PrintLog(TEXT("BusInstance is nullptr"));
        }
    }
    else
    {
        LogHelper::PrintLog(TEXT("World is nullptr"));
    }

    return EventHandler;
}
