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
#include <FPSAbility.h>


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

void AFPSCharacter::Fire()
{
	if (IsBlocked) return; // Character is currently blocked and can't shoot
	GetWeapon()->Fire();
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

float lastAbilityUsed = 0;

void AFPSCharacter::Ability()
{
	if (AbilityClasses[ActiveAbility]) // Temp: Just the first assigned ability, inventory will be added later
	{
		TSubclassOf<AFPSAbility> AbilityClass = AbilityClasses[ActiveAbility];
		float Cooldown = AbilityClass.GetDefaultObject()->Cooldown;
		float Time = GetWorld()->GetTimeSeconds();

		if (Cooldown > 0 && lastAbilityUsed > 0) // larger than 0 so we need to check cooldown
		{
			LogHelper::PrintLog(FString::Printf(TEXT("Check: %f"), lastAbilityUsed + Cooldown));
			if (lastAbilityUsed + Cooldown > Time) // check cooldown
			{
				// Ability is on cooldown
				return;
			}
		}

		LogHelper::PrintLog(FString::Printf(TEXT("Time: %f"), Time));
		lastAbilityUsed = Time;

		// Add your ability activation code here
		
		//ability called here

		FVector AbilityLocation = Mesh1PComponent->GetSocketLocation("hand_r");
		// Use controller rotation which is our view direction in first person
		FRotator AbilityRotation = this->GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this;

		AFPSAbility* Ability = GetWorld()->SpawnActor<AFPSAbility>(AbilityClass, AbilityLocation, AbilityRotation, ActorSpawnParams);
		Ability->TryPlay();
	}
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	lastAbilityUsed = 0;
	UWorld* World = GetWorld();
	// A EventHandler is Spawned for AFPSCharacter which handles the sending of Events, no callback function was register.
	EventHandler = EventBusHelper::SetupAndRegisterEventHandler(World, this, [](UPsEvent* Event) {});
	// The last param ReceiveFunc, can be used to register a callback function for when other Events are sent.

	UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(GetDefaultSubobjectByName(TEXT("Active_Weapon")));
	if (ChildActorComp)
	{
		// Get the child actor from the component
		AActor* ChildActor = ChildActorComp->GetChildActor();
		if (ChildActor)
		{
			// Use Cast to safely cast the actor to your specific class
			Weapon = Cast<AFPSWeapon>(ChildActor);
			if (Weapon)
			{
				// Successfully casted, you can now use Weapon
				Weapon->SetAHolder(this);
				Weapon->SetMesh1PComponent(Mesh1PComponent);
				LogHelper::PrintLog("Successfully found Weapon");
			}
			else
			{
				LogHelper::PrintLog("Failed to cast active Weapon");
			}
		}
		else
		{
			LogHelper::PrintLog("Failed to get child actor from component");
		}
	}
	else
	{
		LogHelper::PrintLog("Failed to find Active_Weapon component");
	}

}

TArray<TSubclassOf<AFPSAbility>> AFPSCharacter::GetAbilities()
{
	return this->AbilityClasses;
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
		DashCooldown->Initialize(DashCooldownInSeconds);
	}
	return DashCooldown;
}
