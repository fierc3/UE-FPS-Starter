// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventHandlerActor.h"
#include "FPSProjectile.generated.h"


class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;


UCLASS()
class AFPSProjectile : public AActor
{
	GENERATED_BODY()

protected:
	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UParticleSystem* ExplosionFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool ShouldExplode;

	UFUNCTION()
	void Explode();	
	
	UFUNCTION()
	void EndProjectile();

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintExplode();

	// -- Events -- //
	AEventHandlerActor* EventHandler;

public:

	AFPSProjectile();

	virtual void BeginPlay() override;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float FireRate;
};

