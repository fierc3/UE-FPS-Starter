// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EventBusHelper.h"
#include <LogHelper.h>

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	LogHelper::PrintLog(TEXT("Spawning Enemy"));
	EventHandler = EventBusHelper::SetupAndRegisterEventHandler(GetWorld(), this, [this](UPsEvent* Event) {
		LogHelper::PrintLog(TEXT("HIT INCOMING"));
		
		// check if its actually me?
		if (Event->Target != this) {
			return;
		}

		Health -= 10.0f;
		LogHelper::PrintLog(FString::Printf(TEXT("Health: %f"), Health));
	});	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health < 1) {

		if (IsDying) {
			LogHelper::PrintLog(TEXT("Enemy DEAD"));
			IsDead = true;
			IsDying = false;
			// play destroy animation
			Destroy();
			return;
		}
		LogHelper::PrintLog(TEXT("Enemy DYING"));
		IsDying = true; 
		Health = 1; // Or else he instantly goes in to death on next tick
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

