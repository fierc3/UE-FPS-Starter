// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequence.h"
#include "LogHelper.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CooldownHelper.h"
#include "EventBusActor.h"
#include "EventHandlerActor.h"
#include "EventBusHelper.h"


AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(Input_Move, ETriggerEvent::Triggered, this, &AFPSCharacter::MoveInput);
	EnhancedInputComponent->BindAction(Input_Look, ETriggerEvent::Triggered, this, &AFPSCharacter::LookInput);

	// Jump exists in the base class, we dont need our own function
	EnhancedInputComponent->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(Input_Fire, ETriggerEvent::Triggered, this, &AFPSCharacter::Fire);
	EnhancedInputComponent->BindAction(Input_Crouch, ETriggerEvent::Triggered, this, &AFPSCharacter::Crouch);
	EnhancedInputComponent->BindAction(Input_Dash, ETriggerEvent::Triggered, this, &AFPSCharacter::Dash);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	// Add mappings for our game, more complex games may have multiple Contexts that are added/removed at runtime
	Subsystem->AddMappingContext(DefaultInputMapping, 0);
}


void AFPSCharacter::ChangeProjectile(int index)
{
	this->currentWeapon = index;
}

int AFPSCharacter::GetCurrentProjectile()
{
	return this->currentWeapon;
}

void AFPSCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (IsLocallyControlled())
	{
		/* Play landed camera anim */
		APlayerController* PC = Cast<APlayerController>(GetController());
		PC->PlayerCameraManager->StartCameraShake(LandedCameraShake);
		
		//UGameplayStatics::PlaySound2D(this, LandedSound);
	}
}

void AFPSCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	// Don't play when this code is running for another player's character (multiplayer)
	if (IsLocallyControlled())
	{
		/* Play jump camera anim */
		APlayerController* PC = Cast<APlayerController>(GetController());
		PC->PlayerCameraManager->StartCameraShake(JumpCameraShake);
		
		//UGameplayStatics::PlaySound2D(this, JumpedSound);
	}
}

void AFPSCharacter::Crouch()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Crouch Called"));
	LogHelper::PrintLog(TEXT("Crouch Called"));

	if (Super::bIsCrouched)
	{
		Super::UnCrouch(false);
	}
	else {
		Super::Crouch(false);
	}
}

void AFPSCharacter::Dash()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent) {
		UCooldownHelper* CooldownHelper = GetDashCooldown();

		if (CooldownHelper->IsCooldownActive()) { // exit if cooldown is active
			return;
		}
		FVector DashDirection  = CharacterMovementComponent->GetLastInputVector().GetSafeNormal();
		int modifier = CharacterMovementComponent->IsFalling() ? 0 : 1;
		LaunchCharacter(DashDirection * DashDistance * DashSpeed * modifier, true, true);


		if (!CooldownHelper->IsCooldownActive() && modifier != 0)
		{
			// Start Dash Cooldown
			CooldownHelper->StartCooldown(GetWorld());
		}
	}
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	// A EventHandler is Spawned for AFPSCharacter which handles the sending of Events, no callback function was register.
	EventHandler = EventBusHelper::SetupAndRegisterEventHandler(World, this, [](UPsEvent* Event) {});
	// The last param ReceiveFunc, can be used to register a callback function for when other Events are sent.
}

void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileClass[this->currentWeapon])
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		FRotator MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass[this->currentWeapon], MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}

	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	
	// Get the animation object for the arms mesh
	UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
	}

	// Play Muzzle FX with an offset to the right (temp offset)
	FVector Offset(0.0, 50.0f, -30.0f); // Adjust the values as needed
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMeshComponent, "Muzzle", Offset);

}

void AFPSCharacter::MoveInput(const FInputActionValue& InputValue)
{
	// Combined input from forward/back (X) and left/right (Y)
	FVector2d MoveValue = InputValue.Get<FVector2d>();
	
	// add movement in that direction
	AddMovementInput(GetActorForwardVector(), MoveValue.X);

	// add movement in that direction
	AddMovementInput(GetActorRightVector(), MoveValue.Y);
}

void AFPSCharacter::LookInput(const FInputActionValue& InputValue)
{
	// Combined input from look up/down (X) and left/right (Y)
	FVector2d LookValue = InputValue.Get<FVector2d>();
	
	float lookScale = 0.25;
	AddControllerYawInput(LookValue.X * lookScale);
	AddControllerPitchInput(LookValue.Y	* lookScale);
}

UCooldownHelper* AFPSCharacter::GetDashCooldown()
{
	if (!DashCooldown)
	{
		DashCooldown = NewObject<UCooldownHelper>();
		DashCooldown->Initialize(DashCooldownInSeconds); // Set the cooldown duration to 5 seconds
	}
	return DashCooldown;
}
