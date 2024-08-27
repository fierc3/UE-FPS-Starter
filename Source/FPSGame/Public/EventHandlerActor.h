// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventHandlerActor.generated.h"

class AEventBusActor;

UCLASS()
class FPSGAME_API AEventHandlerActor : public AActor
{
    GENERATED_BODY()

private:
    int32 ID;  // Unique ID for each EventHandler
    AEventBusActor* Bus;  // Reference to the Bus singleton

public:
    // Default constructor
    AEventHandlerActor();

    // Constructor that takes a pointer to the Bus
    AEventHandlerActor(AEventBusActor* InBus);

    // Method to send a message
    void Send();

    // Method to receive a message with a callback
    void Receive(FSimpleDelegate Delegate);

    // Method to set the Bus pointer after creation
    void SetBus(AEventBusActor* InBus);
};
