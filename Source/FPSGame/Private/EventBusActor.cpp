// Fill out your copyright notice in the Description page of Project Settings.


#include "EventBusActor.h"
#include "Engine/World.h"
#include "LogHelper.h"
#include "EventBusActor.h"

// Initialize the static instance to nullptr
AEventBusActor* AEventBusActor::Instance = nullptr;

AEventBusActor::AEventBusActor()
{
}

void AEventBusActor::BeginPlay()
{
    //Instance = nullptr;
    LogHelper::PrintLog(TEXT("++++++++Begin Play++++++++++"));
    // Optional: Any logic to run when the singleton is first created
    //Listeners = TArray<TPair<AEventHandlerActor*, FSimpleDelegate>>();
}


AEventBusActor* AEventBusActor::GetInstance(UWorld* World)
{
    // Check if the instance already exists
    if (!Instance)
    {
        LogHelper::PrintLog(TEXT("CREATING NEW ISNTANCE OF BUS"));
        // Spawn the Singleton Actor in the world if it doesn't exist
        Instance = World->SpawnActor<AEventBusActor>();
    }
    return Instance;
}

void AEventBusActor::ClearInstance() {
    Instance = nullptr;
}

void AEventBusActor::Register(AEventHandlerActor& Sender, FSimpleDelegate Callback)
{
    LogHelper::PrintLog(TEXT("Registering handler"));
    Listeners.Add(TPair<AEventHandlerActor*, FSimpleDelegate>(&Sender, Callback));
}

void AEventBusActor::SendToAll()
{
    for (auto& Listener : Listeners)
    {
        Listener.Value.ExecuteIfBound();
    }
}
