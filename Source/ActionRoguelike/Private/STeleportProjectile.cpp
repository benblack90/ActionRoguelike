// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASTeleportProjectile::ASTeleportProjectile()
{
	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnActorHit);		
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTeleportProjectile::Teleport, 0.2f);
}



void ASTeleportProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		Teleport();
	}
}

void ASTeleportProjectile::Teleport()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	LastHeading = MovementComp->Velocity;
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	if(ensureAlways(TeleportExitParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportExitParticle, GetActorLocation());
	}	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTeleportProjectile::Teleport_ParticleTimeElapsed, 0.2);
}

void ASTeleportProjectile::Teleport_ParticleTimeElapsed()
{
	if(!GetInstigator()->TeleportTo(GetActorLocation(), GetActorRotation()))
	{
		GetInstigator()->TeleportTo(GetActorLocation() - LastHeading.GetSafeNormal(), GetActorRotation());
	}
	Destroy();
}


