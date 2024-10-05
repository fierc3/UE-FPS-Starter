// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "FPSCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

class UTexture2D;

UCLASS()
class AFPSHUD : public AHUD
{
	GENERATED_BODY()

protected:

	/** Crosshair asset pointer */
	UTexture2D* CrosshairTex;
	UTexture2D* HitmarkerTex;
	float HitmarkerOpacity;
	float HitmarkerScale;

public:

	AFPSHUD();

	virtual void BeginPlay() override;
	
	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	AFPSCharacter* PlayerCharacter;
	// -- Events -- //
	AEventHandlerActor* EventHandler;
	void UpdateHitmarkerOpacity();
	FTimerHandle HitmarkerTimerHandle;
};

