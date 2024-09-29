// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CooldownHelper.h"

// Included for struct FInputActionInstance (Enhanced Input)
#include "InputAction.h"
#include <FPSWeapon.h>
#include "FPSAbility.h"
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

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> LandedCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> JumpCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AFPSWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<TSubclassOf<AFPSAbility>> AbilityClasses;

public:
	AFPSCharacter();

	UFUNCTION(BlueprintCallable, Category = "Dash")
	UCooldownHelper* GetDashCooldown();

	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnJumped_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsBlocked = false;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	TArray<TSubclassOf<AFPSAbility>> GetAbilities();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float ActiveAbility = 0;

protected:

	void MoveInput(const FInputActionValue& InputValue);

	void LookInput(const FInputActionValue& InputValue);
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Fire();

	void StartShooting();

	void StopShooting();

	void Crouch();
	
	void Dash();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void Ability();

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	UCooldownHelper* DashCooldown;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashCooldownInSeconds = 1.0f;

public:
	/** Returns Mesh1P subobject **/
	UFUNCTION(BlueprintCallable, Category = "Mesh")
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UFUNCTION(BlueprintCallable, Category = "Camera")
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

	AFPSWeapon* GetWeapon() const { return Weapon; }

private:
	bool IsShooting = false;
};

