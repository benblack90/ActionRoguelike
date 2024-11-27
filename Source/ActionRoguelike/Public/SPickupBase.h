// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickupBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASPickupBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickupBase();

protected:

	bool bActive;
	float TimeToRespawn;
	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Deactivate() {};
	virtual void Respawn() {};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
