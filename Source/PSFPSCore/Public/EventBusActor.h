// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PsEvent.h"
#include "EventBusActor.generated.h"

// Forward declaration of AEventHandler
class AEventHandlerActor;    

UCLASS()
class  AEventBusActor : public AActor
{
    GENERATED_BODY()

private:
    static AEventBusActor* Instance;  // Static instance of the Singleton

    // Private constructor to prevent direct instantiation
    AEventBusActor();

    // List of registered event handlers
    TArray<TPair<AEventHandlerActor*, FEventDelegate>> Listeners;

public:
    // Singleton access method
    static AEventBusActor* GetInstance(UWorld* World);
    static void ClearInstance();

    // Override BeginPlay to initialize the singleton
    virtual void BeginPlay() override;  

    // Register an event handler with a callback
    void Register(AEventHandlerActor& Sender, FEventDelegate Callback);

    // Send event to all registered handlers
    void SendToAll(UPsEvent* ev);
};
