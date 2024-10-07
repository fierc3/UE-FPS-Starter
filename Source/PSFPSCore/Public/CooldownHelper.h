// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CooldownHelper.generated.h"

/**
 * 
 */
UCLASS()
class UCooldownHelper : public UObject
{
	GENERATED_BODY()

    private:
        float CooldownDuration;
        bool bIsOnCooldown;
        FTimerHandle CooldownTimerHandle;

    public:
        void Initialize(float InCooldownDuration);
        UFUNCTION(BlueprintCallable, Category = "Cooldown")
        void StartCooldown(UWorld* World);
        UFUNCTION(BlueprintCallable, Category = "Cooldown")
        bool IsCooldownActive() const;
        UFUNCTION(BlueprintCallable, Category = "Cooldown")
        float GetRemainingCooldownTime(UWorld* World) const;

    private:
        void ResetCooldown();
};
