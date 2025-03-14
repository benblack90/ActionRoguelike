// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SProjectileBase.h"
#include "CoreMinimal.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitWorldParticle;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> Shake;
	UPROPERTY(EditAnywhere, Category="Camera Shake")
	float ShakeInnerRadius;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	float ShakeOuterRadius;

	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
