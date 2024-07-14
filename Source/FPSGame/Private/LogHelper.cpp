// Fill out your copyright notice in the Description page of Project Settings.


#include "LogHelper.h"

LogHelper::LogHelper()
{
}

LogHelper::~LogHelper()
{
}

void LogHelper::PrintLog(const FString& Message, const FColor* Color, float Duration)
{
    FColor FinalColor = (Color != nullptr) ? *Color : GetColorForMessage(Message);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, Duration, FinalColor, Message);
    }
}

FColor LogHelper::GetColorForMessage(const FString& Message)
{
    if (Message.IsEmpty())
    {
        return FColor::White; // Default color for empty messages
    }

    // Use a hash function to determine the color
    uint32 Hash = FCrc::StrCrc32(*Message.Left(1));
    switch (Hash % 4)
    {
        case 0: return FColor::Red;
        case 1: return FColor::Blue;
        case 2: return FColor::Green;
        case 3: return FColor::Cyan;
        default: return FColor::Yellow; // Default color if no match
    }
}