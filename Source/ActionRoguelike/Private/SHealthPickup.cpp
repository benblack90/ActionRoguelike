// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPickup.h"
#include "SAttributeComponent.h"

ASHealthPickup::ASHealthPickup()
	:HealValue{10.0f}
{ }



void ASHealthPickup::Interact_Implementation(APawn* InstigatorPawn)
{
	if(bActive)
	{
		if(InstigatorPawn)
		{
			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (AttributeComp)
			{
				if (AttributeComp->ApplyHealthChange(HealValue))
				{
					Deactivate();
				}
			}
		}		
	}
}

void ASHealthPickup::Deactivate()
{
	bActive = false;
	MeshComp->SetVisibility(false);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASHealthPickup::Respawn, TimeToRespawn);
}


void ASHealthPickup::Respawn()
{
	MeshComp->SetVisibility(true);
	GetWorldTimerManager().ClearTimer(TimerHandle);
	bActive = true;
}
