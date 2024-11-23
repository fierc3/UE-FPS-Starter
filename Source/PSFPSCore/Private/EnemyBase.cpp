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
		// check if its actually me?
		if (Event->Target != this) {
			return;
		}
		Health -= 10.0f;
		LogHelper::PrintLog(FString::Printf(TEXT("Health: %f"), Health));
		BlueprintPostHit(Event);
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
		UPsEvent* Event = NewObject<UPsEvent>(this);
		Event->EventType = EEventType::IsDying;
		Event->Value = 1.0f;
		BlueprintIsDying(Event);
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FGenericTeamId AEnemyBase::GetGenericTeamId() const
{
	return FGenericTeamId(100); // 100 for base Enemy
}

ETeamAttitude::Type AEnemyBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const uint8 TeamId = GetGenericTeamId();
	if (const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(&Other))
	{
		FGenericTeamId OtherTeamID = OtherTeamAgent->GetGenericTeamId();
		if (OtherTeamID == 1)
		{
			return ETeamAttitude::Hostile;
		}
		else if (OtherTeamID > TeamId && OtherTeamID < (TeamId+100))
		{
			return ETeamAttitude::Friendly;
		}
	}
	return ETeamAttitude::Neutral;
}

