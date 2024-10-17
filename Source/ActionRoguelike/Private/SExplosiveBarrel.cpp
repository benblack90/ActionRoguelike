// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetSimulatePhysics(true);
	RootComponent = Mesh;

	RForceComp = CreateDefaultSubobject<URadialForceComponent>("RForceComp");
	RForceComp->bImpulseVelChange = true;
	RForceComp->ImpulseStrength = 1000.0f;
	RForceComp->Radius = 500.0f;
	RForceComp->SetupAttachment(Mesh);
	
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::Explode);
	
}

//when passing a function to a delegate, the signature has to match that of the delegate. As a result, the signature below has 5 parameters, matching what
//OnComponentHit.AddDynamic needs
void ASExplosiveBarrel::Explode(UPrimitiveComponent* Pc, AActor* Act, UPrimitiveComponent* Pc2, FVector incoming, const FHitResult& Hr)
{
	RForceComp->FireImpulse();
}



// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

