// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <EventHandlerActor.h>
#include <FPSProjectile.h>
#include "FPSWeapon.generated.h"

/**
 * 
 */

UCLASS()
class  AFPSWeapon : public AActor
{
	GENERATED_BODY()

protected:

	// -- Events -- //
	AEventHandlerActor* EventHandler;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* GunMeshComponent;		
	
	UStaticMeshComponent* StaticMeshComponent;

public:
	AFPSWeapon();

	virtual void BeginPlay() override;

	/** Fires a projectile. */
	bool Fire();

	/** Returns Mesh1P subobject **/
	UStaticMeshComponent* GetGunMesh() const { return GunMeshComponent; }
	UFUNCTION(BlueprintCallable, Category = "Holder")
	void SetMesh1PComponent(USkeletalMeshComponent* NewMesh1PComponent)	{ Mesh1PComponent = NewMesh1PComponent; }
	UFUNCTION(BlueprintCallable, Category = "Holder")
	void SetAHolder(ACharacter* NewAHolder) { AHolder = NewAHolder; }

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TArray<TSubclassOf<AFPSProjectile>> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float delayAfterMuzzle = 0.02f;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ChangeProjectile(int index);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetCurrentProjectile();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnWeaponFired();

protected:

	void FireEmptyBullet();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int MaxBullets = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int CurrentBullets = MaxBullets;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		USoundBase* EmptyFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		USoundBase* ReloadSound;

	/** AnimMontage to play each time we reload */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimSequence* ReloadAnimation;

	UFUNCTION()
	void OnReloadAnimationFinished();

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool IsReloading = false;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintReloadStart();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintReloadEnd();

private:
	FTimerHandle ReloadTimerHandle;
	int CurrentProjectile = 0;
	float LastFireTime = 0;
	
	USkeletalMeshComponent* Mesh1PComponent; // I need a setter for this, in c++ and blueprint
	ACharacter* AHolder; // setter for this too
};
