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
class FPSGAME_API AFPSWeapon : public AActor
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
	void Fire();

	/** Returns Mesh1P subobject **/
	UStaticMeshComponent* GetGunMesh() const { return GunMeshComponent; }
	void SetMesh1PComponent(USkeletalMeshComponent* NewMesh1PComponent)	{ Mesh1PComponent = NewMesh1PComponent; }
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

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ChangeProjectile(int index);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetCurrentProjectile();

protected:

	void FireEmptyBullet();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Reload();

	UPROPERTY(EditAnywhere, Category = "Weapon")
		int MaxBullets = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
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

private:
	FTimerHandle ReloadTimerHandle;
	bool IsReloading = false;
	int CurrentProjectile = 0;

	USkeletalMeshComponent* Mesh1PComponent; // I need a setter for this, in c++ and blueprint
	ACharacter* AHolder; // setter for this too
};
