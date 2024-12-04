// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSWeapon.h"
#include <Kismet/GameplayStatics.h>
#include "EventBusHelper.h"
#include "GameFramework/Character.h"
#include <FPSCharacter.h>


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


bool AFPSWeapon::Fire()
{
	if (IsReloading) return false; // Can only fire when fully reloaded

	if (CurrentBullets < 1)
	{
		FireEmptyBullet();
		//Reload();
		return false;
	}

	// Check if projectile class exists
	if (ProjectileClass[this->CurrentProjectile])
	{
		float  FireRate = ProjectileClass[this->CurrentProjectile].GetDefaultObject()->FireRate;

		if (GetWorld()->GetTimeSeconds() - LastFireTime < FireRate && FireRate > 0 && LastFireTime > 0)
		{
			return false;
		}

		LastFireTime = GetWorld()->GetTimeSeconds();

		FVector MuzzleLocation = StaticMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		FRotator MuzzleRotation = this->AHolder->GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this->AHolder;

		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass[this->CurrentProjectile], MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		CurrentBullets -= 1;

		OnWeaponFired(); // So the blueprint can do some stuff after a weapon has been fired
		
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (PlayerCharacter)
		{
			// Cast to AFPSCharacter
			AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(PlayerCharacter);
			if (FPSCharacter)
			{
				FPSCharacter->OnWeaponFired();
			}
		}
	}

	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

	// Get the animation object for the arms mesh

	if (MuzzleFlash) {
		FVector MuzzleLocation = StaticMeshComponent->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = StaticMeshComponent->GetSocketRotation("Muzzle");
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, MuzzleLocation, MuzzleRotation);
	}

	return true;

	/*
	UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
	if (AnimInstance)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [this, AnimInstance]() {
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}), delayAfterMuzzle, false);
	}
	*/
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
	BlueprintReloadEnd();
}

void AFPSWeapon::Reload()
{
	if (CurrentBullets == MaxBullets
		|| IsReloading) {
		return;
	}

	IsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFPSWeapon::OnReloadAnimationFinished, 1, false);

	/*UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
	if (AnimInstance && ReloadAnimation)
	{
		IsReloading = true;
		AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadAnimation, "Arms", 0.0f);

		// Get the duration of the animation
		float AnimationDuration = ReloadAnimation->GetPlayLength();

		// Set a timer to call OnReloadAnimationFinished after the animation duration
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFPSWeapon::OnReloadAnimationFinished, AnimationDuration, false);
	}*/
	BlueprintReloadStart();
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
}
