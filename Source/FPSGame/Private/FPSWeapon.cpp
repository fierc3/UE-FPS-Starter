// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSWeapon.h"
#include <Kismet/GameplayStatics.h>
#include "EventBusHelper.h"
#include "GameFramework/Character.h"


AFPSWeapon::AFPSWeapon()
{
	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FP_Gun"));
	//StaticMeshComponent = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("StaticMesh")));
}

void AFPSWeapon::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	EventHandler = EventBusHelper::SetupAndRegisterEventHandler(World, this, [](UPsEvent* Event) {});
	// The last param ReceiveFunc, can be used to register a callback function for when other Events are sent.
	StaticMeshComponent = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("StaticMesh")));
	if (StaticMeshComponent)
	{
		// Do something with the StaticMeshComponent
		UE_LOG(LogTemp, Warning, TEXT("StaticMesh component found!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StaticMesh component not found."));
	}
}

void AFPSWeapon::Fire()
{
	if (IsReloading) return; // Can only fire when fully reloaded

	if (CurrentBullets < 1)
	{
		FireEmptyBullet();
		return;
	}

	// try and fire a projectile
	if (ProjectileClass[this->CurrentProjectile])
	{
		
		FVector MuzzleLocation = StaticMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		FRotator MuzzleRotation = this->AHolder->GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this->AHolder;

		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass[this->CurrentProjectile], MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		CurrentBullets -= 1;
	}

	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

	// Get the animation object for the arms mesh
	UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
	}

	if (MuzzleFlash) {
		FVector MuzzleLocation = StaticMeshComponent->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = StaticMeshComponent->GetSocketRotation("Muzzle");
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, MuzzleLocation, MuzzleRotation);
	}

}

void AFPSWeapon::ChangeProjectile(int index)
{
	this->CurrentProjectile = index;
}

int AFPSWeapon::GetCurrentProjectile()
{
	return this->CurrentProjectile;
}


void AFPSWeapon::FireEmptyBullet()
{
	// play empty bullet sound
	UGameplayStatics::PlaySoundAtLocation(this, EmptyFireSound, GetActorLocation());
}

void AFPSWeapon::OnReloadAnimationFinished()
{
	IsReloading = false;
	CurrentBullets = MaxBullets;
}

void AFPSWeapon::Reload()
{
	if (CurrentBullets == MaxBullets
		|| IsReloading) {
		return;
	}

	UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
	if (AnimInstance && ReloadAnimation)
	{
		IsReloading = true;
		AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadAnimation, "Arms", 0.0f);

		// Get the duration of the animation
		float AnimationDuration = ReloadAnimation->GetPlayLength();

		// Set a timer to call OnReloadAnimationFinished after the animation duration
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFPSWeapon::OnReloadAnimationFinished, AnimationDuration, false);
	}
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
}
