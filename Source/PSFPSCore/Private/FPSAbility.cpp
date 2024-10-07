// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAbility.h"

AFPSAbility::AFPSAbility()
{
}

AFPSAbility::~AFPSAbility()
{
}

bool AFPSAbility::TryPlay()
{
	// check things like cooldown
	// If okay then Play
	Play();

	return true;
}

void AFPSAbility::Play()
{
	Pre();
	Main();
	Post();
}

void AFPSAbility::Pre()
{
	BlueprintPre();
}

void AFPSAbility::Main()
{
	BlueprintMain();
}

void AFPSAbility::Post()
{
	BlueprintPost();
}
