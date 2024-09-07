// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CooldownHelper.h"

// Included for struct FInputActionInstance (Enhanced Input)
#include "InputAction.h"

#include "FPSCharacter.generated.h"

class UInputMappingContext;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class UParticleSystem;
class AEventHandlerActor;


UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	// -- Events -- //
	AEventHandlerActor* EventHandler;

	// -- Enhanced Input -- //

	/* Holds collection of currently active and available InputActions */
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category= "Input")
	UInputAction* Input_Move;
	
	UPROPERTY(EditDefaultsOnly, Category= "Input")
	UInputAction* Input_Look;
	
	UPROPERTY(EditDefaultsOnly, Category= "Input")
	UInputAction* Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category= "Input")
	UInputAction* Input_Fire;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Crouch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Dash;

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> LandedCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> JumpCameraShake;

public:
	AFPSCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TArray<TSubclassOf<AFPSProjectile>> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UParticleSystem* MuzzleFlash;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ChangeProjectile(int index);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetCurrentProjectile();

	UFUNCTION(BlueprintCallable, Category = "Dash")
	UCooldownHelper* GetDashCooldown();

	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnJumped_Implementation() override;

protected:
	
	/** Fires a projectile. */
	void Fire();

	void MoveInput(const FInputActionValue& InputValue);

	void LookInput(const FInputActionValue& InputValue);
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void BeginPlay() override;

	void Crouch();
	
	void Dash();

	void FireEmptyBullet();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	UCooldownHelper* DashCooldown;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashCooldownInSeconds = 1.0f;

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

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

private:
	int currentWeapon = 0;
	FTimerHandle ReloadTimerHandle;
	bool IsReloading = false;
};

