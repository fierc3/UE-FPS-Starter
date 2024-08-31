// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "LogHelper.h"
#include <EventBusHelper.h>
#include "EnemyBase.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"



AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}


void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSProjectile::EndProjectile, 3.0f, false);

	EventHandler = EventBusHelper::SetupAndRegisterEventHandler(GetWorld(), this, [this](UPsEvent* Event) {});
}

void AFPSProjectile::EndProjectile()
{
	if (this->ShouldExplode) {
		this->Explode();
	}
	else {
		Destroy();
	}
}

void AFPSProjectile::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f));

	// Allow BP to trigger additional logic
	BlueprintExplode();

	Destroy();
}


void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// also check if of type enemy
	if ((OtherActor) && (OtherActor != this) && OtherActor->IsA(AEnemyBase::StaticClass())) {

		LogHelper::PrintLog(OtherActor->GetName());
		UPsEvent* HitEvent = NewObject<UPsEvent>(this);
		HitEvent->EventType = EEventType::Hit;
		HitEvent->Value = 10.0f;
		HitEvent->Origin = this;
		HitEvent->Target = OtherActor;
		EventHandler->Send(HitEvent);
	}	

	FVector Location = Hit.Location; // Replace with your hit location
	FRotator Rotation = FRotationMatrix::MakeFromZ(Hit.ImpactNormal).Rotator();

	ADecalActor* DecalActor = GetWorld()->SpawnActor<ADecalActor>(Location, Rotation);
	if (DecalActor)
	{
		// Set the decal material
		UMaterialInterface* DecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/Decals/BulletDecal.BulletDecal'"));
		DecalActor->SetDecalMaterial(DecalMaterial);
		DecalActor->SetLifeSpan(10.0f); // Decal will disappear after 10 seconds
		// Adjust the decal size
		FVector DecalSize = FVector(32.0f, 10.0f, 10.0f); // Adjust these values to make the decal smaller
		DecalActor->GetDecal()->DecalSize = DecalSize;

		/*
		// Adjust the decal transform to keep it centered
		FTransform DecalTransform = DecalActor->GetTransform();
		DecalTransform.SetLocation(FVector(DecalTransform.GetLocation().X, DecalTransform.GetLocation().Y - 5, DecalTransform.GetLocation().Z - 5));
		DecalActor->SetActorTransform(DecalTransform);
		*/
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn decal actor"));
	}

	// Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor) && (OtherActor != this) && (OtherComp) && OtherComp->IsSimulatingPhysics())
	{
		float RandomIntensity = FMath::RandRange(200.0f, 500.0f);

		OtherComp->AddImpulseAtLocation(GetVelocity() * RandomIntensity, GetActorLocation());

		FVector Scale = OtherComp->GetComponentScale();
		Scale *= 0.8f;

		if (Scale.GetMin() < 0.5f)
		{
			OtherActor->Destroy();
		}
		else
		{
			OtherComp->SetWorldScale3D(Scale);
		}

		UMaterialInstanceDynamic* MatInst = OtherComp->CreateDynamicMaterialInstance(0);
		if (MatInst)
		{
			MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
		}

		// it has hit an physics object, it should end now
		this->EndProjectile();
		return;
	}
	
	if (!ProjectileMovement->bShouldBounce) {
		// It's not a bounce projectile, so it should be destroyed after it hitting something for hte first time
		this->EndProjectile();
	}
}