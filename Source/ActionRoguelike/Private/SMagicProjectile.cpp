// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include <SGameplayFunctionLibrary.h>

// Sets default values
ASMagicProjectile::ASMagicProjectile()
	: ShakeInnerRadius{25.0f}, ShakeOuterRadius{200.0f}
{
	Damage = -20.0f;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult);
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitWorldParticle, GetActorLocation(), GetActorRotation());
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());
	}

	if(Shake)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), Shake, GetActorLocation(), ShakeInnerRadius, ShakeOuterRadius);
	}
	
	Destroy();
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

