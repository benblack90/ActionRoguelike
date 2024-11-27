// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	if(Delta > 0 && Health >= MaxHealth)
	{
		return false;
	}

	Health = FMath::Clamp(Health + Delta, 0.0f, 100.0f);

	OnHealthChanged.Broadcast(nullptr, this, Health, MaxHealth, Delta);

	//Change this when it's possible for this to fail!
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0;
}

