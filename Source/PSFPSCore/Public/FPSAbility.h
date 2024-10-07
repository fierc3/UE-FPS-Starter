// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CooldownHelper.h"
#include "FPSAbility.generated.h"

/**
 * 
 */
UCLASS()
class  AFPSAbility : public AActor
{
	GENERATED_BODY()

public:
	AFPSAbility();
	~AFPSAbility();
	bool TryPlay();
	void Play();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Cooldown = -1;
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

