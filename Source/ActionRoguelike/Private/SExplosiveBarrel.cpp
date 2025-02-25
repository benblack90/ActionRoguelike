// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "SAttributeComponent.h"
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
	
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Mesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::Explode);
}

//when passing a function to a delegate, the signature has to match that of the delegate. As a result, the signature below has 5 parameters, matching what
//OnComponentHit.AddDynamic needs
void ASExplosiveBarrel::Explode(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, FVector Normal, const FHitResult& HitResult)
{
	RForceComp->FireImpulse();
	if(OtherActor)
	{
		USAttributeComponent* AttComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttComp)
		{
			AttComp->ApplyHealthChange(this,-35);
		}		
	}

	//ah, macros within macros! UE_LOG is how you log to console. TEXT is a macro for writing ... well, text
	UE_LOG(LogTemp, Log, TEXT("Barrel exploded"));

	//GetNameSafe is a nice way of reading data without checking for null (in this case, getting the name). It's a dereferenced pointer, because it expects a char array
	UE_LOG(LogTemp, Warning, TEXT("Other Actor: %s at gametime %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location %s"), *HitResult.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), HitResult.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}



// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

