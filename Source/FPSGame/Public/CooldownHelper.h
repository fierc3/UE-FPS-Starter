// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CooldownHelper.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UCooldownHelper : public UObject
{
	GENERATED_BODY()

    private:
        float CooldownDuration;
        bool bIsOnCooldown;
        FTimerHandle CooldownTimerHandle;

    public:
        void Initialize(float InCooldownDuration);
        void StartCooldown(UWorld* World);
        bool IsCooldownActive() const;
        float GetRemainingCooldownTime(UWorld* World) const;

    private:
        void ResetCooldown();
};
