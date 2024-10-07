// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PsEvent.generated.h"

DECLARE_DELEGATE_OneParam(FEventDelegate, UPsEvent*);

// Enum to represent different types of events
UENUM(BlueprintType)
enum class EEventType : uint8
{
    Hit     UMETA(DisplayName = "Hit"),
    Action  UMETA(DisplayName = "Action"),
    // Add other event types as needed
};

UCLASS(Blueprintable)
class  UPsEvent : public UObject
{
    GENERATED_BODY() // this is fine apparently :shrug:

public:
    // The type of event
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Event")
        EEventType EventType;

    // A generic value associated with the event
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Event")
        float Value;

    // The originator of the event
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Event")
        AActor* Origin;

    // The target of the event
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Event")
        AActor* Target;

    // Additional data can be added here, depending on the event type
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Event")
    FName AdditionalData;

    // Adds additional hit result data, if available
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Event")
    FHitResult HitResult;

    // Constructor to initialize the event
    UPsEvent()
    {
        EventType = EEventType::Hit; // Default type
        Value = 0.0f;
        Origin = nullptr;
        Target = nullptr;
    }

    UPsEvent(EEventType InEventType, float InValue, AActor* InOrigin, AActor* InTarget, FName InAdditionalData)
        : EventType(InEventType), Value(InValue), Origin(InOrigin), Target(InTarget), AdditionalData(InAdditionalData)
    {
    }
};