// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"


class URadialForceComponent;
class UCapsuleComponent;

UCLASS()
class ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	URadialForceComponent* RForceComp;

	UFUNCTION()
	void Explode(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, FVector Normal, const FHitResult& HitResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
