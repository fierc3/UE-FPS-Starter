// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CooldownHelper.h"
#include "FPSAbility.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAbility : public AActor
{
	GENERATED_BODY()

public:
	AFPSAbility();
	~AFPSAbility();
	bool TryPlay();
	void Play();
protected:
	UFUNCTION(BlueprintCallable, Category = "Cooldown")
	UCooldownHelper* GetTimerCooldown(float defaultCooldown);
	UPROPERTY(EditAnywhere, Category = "Cooldown")
	UCooldownHelper* TimeCooldown;
private:
	void Pre();
	void Main();
	void Post();
public:
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintPre();
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintMain();
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintPost();
};

