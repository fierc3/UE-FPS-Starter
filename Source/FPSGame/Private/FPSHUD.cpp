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
		int projectile = PlayerCharacter->GetCurrentProjectile();

		// Draw the current projectile at the bottom left of the screen
		FString AmmoText = FString::Printf(TEXT("Projectile: %d"), projectile + 1);
		FVector2D AmmoPosition(50, Canvas->ClipY - 50); // Adjust the position as needed
		UFont* font = GEngine->GetLargeFont();
		// change font here
		FCanvasTextItem TextItem(AmmoPosition, FText::FromString(AmmoText), font, FLinearColor::White);
		// Make it bigger
		TextItem.Scale = FVector2D(1.5f, 1.5f);
		Canvas->DrawItem(TextItem);
	}

}
