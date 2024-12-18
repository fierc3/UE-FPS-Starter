// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "LogHelper.h"
#include "FPSCharacter.h"
#include "EventBusHelper.h"
#include "Kismet/GameplayStatics.h"
#include <EnemyBase.h>

AFPSHUD::AFPSHUD()
{
	// Set the crosshair texture (not normally recommended to link content through a hardcoded line like below)
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/UI/FirstPersonCrosshairSimple"));
	CrosshairTex = CrosshairTexObj.Object;

	// Initialize hitmarker texture (assuming you have a texture asset named "HitmarkerTex")
	static ConstructorHelpers::FObjectFinder<UTexture2D> HitmarkerTexObj(TEXT("/Game/UI/FirstPersonHitmarker"));
	HitmarkerTex = HitmarkerTexObj.Object;

	HitmarkerOpacity = 0.0f;
	HitmarkerScale = 0.65f;
	HitmarkerColor = FLinearColor::White;
}

void AFPSHUD::BeginPlay()
{
	PlayerCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter) {
		LogHelper::PrintLog("HUD Started for " + PlayerCharacter->GetName());
	}

	EventHandler = EventBusHelper::SetupAndRegisterEventHandler(GetWorld(), this, [this](UPsEvent* Event) {
		if (Event->Target->IsA(AEnemyBase::StaticClass())) {
			AEnemyBase* Enemy = Cast<AEnemyBase>(Event->Target);
			if (Enemy && !Enemy->IsDead)
			{
				LogHelper::PrintLog(FString::Printf(TEXT("Enemy health, during hitmarker: %f"), Enemy->Health));

				if (Enemy->Health < 0 || Enemy->IsDying) {
					HitmarkerColor = FLinearColor::Red;
				}
				else {
					HitmarkerColor = FLinearColor::White;
				}

				GetWorld()->GetTimerManager().ClearTimer(HitmarkerTimerHandle);
				HitmarkerOpacity = 1.0f; // We want to show the hitmarker now
				GetWorld()->GetTimerManager().SetTimer(HitmarkerTimerHandle, this, &AFPSHUD::UpdateHitmarkerOpacity, 0.05f, true);
			}
		}

	});
}


void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair
	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const float ScaleFactor = 0.7f;
	// calculate scaled dimensions
	const FVector2D ScaledDimensions(CrosshairTex->GetSurfaceWidth() * ScaleFactor, CrosshairTex->GetSurfaceHeight() * ScaleFactor);
	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	//const FVector2D CrosshairDrawPosition(Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f));
	const FVector2D CrosshairDrawPosition(Center.X - (ScaledDimensions.X * 0.5f), Center.Y - (ScaledDimensions.Y * 0.5f));
	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->GetResource(), ScaledDimensions, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

	// Draw hitmarker
	if (HitmarkerTex && HitmarkerOpacity > 0.0f)
	{
		const FVector2D HitmarkerSize(HitmarkerTex->GetSurfaceWidth() * HitmarkerScale, HitmarkerTex->GetSurfaceHeight() * HitmarkerScale);
		const FVector2D HitmarkerDrawPosition(Center.X - (HitmarkerSize.X * 0.5f), Center.Y - (HitmarkerSize.Y * 0.5f));
		FCanvasTileItem HitmarkerItem(HitmarkerDrawPosition, HitmarkerTex->GetResource(), HitmarkerSize, HitmarkerColor.CopyWithNewOpacity(HitmarkerOpacity));
		HitmarkerItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(HitmarkerItem);
	}

	if (PlayerCharacter)
	{
		//int projectile = PlayerCharacter->GetWeapon()->GetCurrentProjectile();
		int projectile = 0;
		// Draw the current projectile at the bottom left of the screen
		FString AmmoText = FString::Printf(TEXT("Projectile: %d"), projectile + 1);
		FVector2D AmmoPosition(50, Canvas->ClipY - 50); // Adjust the position as needed
		FCanvasTextItem TextItem(AmmoPosition, FText::FromString(AmmoText), GEngine->GetLargeFont(), FLinearColor::White);
		// Make it bigger
		Canvas->DrawItem(TextItem);

		// Draw the cooldown state
		UCooldownHelper* dashCoolDown = PlayerCharacter->GetDashCooldown();	
		float dashRemaining = dashCoolDown->GetRemainingCooldownTime(GetWorld());
		FString CooldownText = FString::Printf(TEXT("Dash: %.1f"), dashRemaining);
		FVector2D CooldownPosition(150, Canvas->ClipY - 50); // Adjust the position as needed
		FCanvasTextItem CdTextItem(CooldownPosition, FText::FromString(CooldownText), GEngine->GetLargeFont(), dashRemaining > 0.0f ? FLinearColor::Red : FLinearColor::Green);
		// Make it bigger
		Canvas->DrawItem(CdTextItem);
	}
}

void AFPSHUD::UpdateHitmarkerOpacity()
{
	if (HitmarkerOpacity > 0.0f)
	{
		HitmarkerOpacity -= 0.15f; // Adjust the fade speed as needed
		if (HitmarkerOpacity < 0.0f)
		{
			HitmarkerOpacity = 0.0f;
			GetWorld()->GetTimerManager().ClearTimer(HitmarkerTimerHandle); // Stop the timer when opacity reaches 0
		}
	}
}
