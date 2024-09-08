// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "LogHelper.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

AFPSHUD::AFPSHUD()
{
	// Set the crosshair texture (not normally recommended to link content through a hardcoded line like below)
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/UI/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AFPSHUD::BeginPlay()
{
	PlayerCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	LogHelper::PrintLog("HUD Started for " + PlayerCharacter->GetName());
}


void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition(Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->GetResource(), FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

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
