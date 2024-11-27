// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Teleport")
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Blackhole")
	TSubclassOf<AActor> BlackholeProjectileClass;

	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* CastEffect;
	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void MoveForward(float Val);
	void MoveRight(float Val);
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void Teleport();
	void Teleport_TimeElapsed();
	void Blackhole();
	void Blackhole_TimeElapsed();
	void PrimaryInteract();
	FRotator CorrectProjectileAngle(const FVector& StartPoint) const;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* UOwningComp, float NewHealth, float MaxHealth, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
