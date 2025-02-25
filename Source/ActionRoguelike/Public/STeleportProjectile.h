// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportProjectile.generated.h"

class UFXSystemAsset;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()

	ASTeleportProjectile();

	virtual void BeginPlay() override;

protected:

	FTimerHandle TimerHandle;
	UPROPERTY(EditAnywhere)
	UParticleSystem* TeleportExitParticle;
	FVector LastHeading;

	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void Teleport();
	void Teleport_ParticleTimeElapsed();
};
