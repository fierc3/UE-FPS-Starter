// Fill out your copyright notice in the Description page of Project Settings.


#include "EventHandlerActor.h"
#include "EventBusActor.h"
#include "LogHelper.h"

// Default constructor
AEventHandlerActor::AEventHandlerActor()
    : Bus(nullptr)  // Initialize the Bus pointer to nullptr
{
    ID = FMath::Rand();  // Generate a random ID for this EventHandler
}

// Constructor that initializes the Bus pointer
AEventHandlerActor::AEventHandlerActor(AEventBusActor* InBus)
    : Bus(InBus)  // Initialize the Bus pointer
{
    ID = FMath::Rand();  // Generate a random ID for this EventHandler
}

// Method to send a message
void AEventHandlerActor::Send()
{
    LogHelper::PrintLog(TEXT("### SENDING"));
    Bus->SendToAll();  // Send to all registered handlers through the Bus
}

// Method to receive a message with a callback
void AEventHandlerActor::Receive(FSimpleDelegate Delegate)
{
    UE_LOG(LogTemp, Warning, TEXT("### RECEIVED by %d"), ID);
    Delegate.ExecuteIfBound();  // Execute the provided delegate
}

// Method to set the Bus pointer after creation
void AEventHandlerActor::SetBus(AEventBusActor* InBus)
{
    Bus = InBus;
}