// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASTeleportProjectile::ASTeleportProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASTeleportProjectile::OnActorOverlap);		
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTeleportProjectile::Teleport, 0.2f);
}



void ASTeleportProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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


