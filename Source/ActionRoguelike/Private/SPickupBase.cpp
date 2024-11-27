// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupBase.h"

#include "Components/SphereComponent.h"

// Sets default values
ASPickupBase::ASPickupBase()
	:bActive{true}, TimeToRespawn{10.0f}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void ASPickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

