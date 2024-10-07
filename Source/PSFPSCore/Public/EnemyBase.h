// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EventHandlerActor.h"
#include "EnemyBase.generated.h"

UCLASS()
class AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// -- Events -- //
	AEventHandlerActor* EventHandler;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool IsDying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool IsDead;

	/** Pawn mesh: Enemy  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintPostHit(UPsEvent* Event);
};
