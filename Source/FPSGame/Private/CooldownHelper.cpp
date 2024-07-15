// Fill out your copyright notice in the Description page of Project Settings.

#include "CooldownHelper.h"
#include "TimerManager.h"

void UCooldownHelper::Initialize(float InCooldownDuration)
{
    CooldownDuration = InCooldownDuration;
    bIsOnCooldown = false;
}

void UCooldownHelper::StartCooldown(UWorld* World)
{
    if (World)
    {
        bIsOnCooldown = true;
        World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UCooldownHelper::ResetCooldown, CooldownDuration, false);
    }
}

bool UCooldownHelper::IsCooldownActive() const
{
    return bIsOnCooldown;
}

void UCooldownHelper::ResetCooldown()
{
    bIsOnCooldown = false;
}

float UCooldownHelper::GetRemainingCooldownTime(UWorld* World) const
{
    if (World && bIsOnCooldown)
    {
        return World->GetTimerManager().GetTimerRemaining(CooldownTimerHandle);
    }
    return 0.0f;
}