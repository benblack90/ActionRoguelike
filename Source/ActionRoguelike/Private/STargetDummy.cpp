// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "SAttributeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}


// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* UOwningComp, float NewHealth, float MaxHealth, float Delta)
{
	if(Delta < 0)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}	
}


