// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include <SGameModeBase.h>

//another console variable (see the one in SGameModeBase.cpp), this time as a damage multiplier
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global damage modifier for attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnAnyThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, MaxHealth, ActualDelta);

	//if dead, call gamemodebase's OnActorKilled for player credits
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	//Change this when it's possible for this to fail!
	return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0;
}

float USAttributeComponent::GetHealthAsFraction()
{
	return Health / MaxHealth;
}

bool USAttributeComponent::IsMaxHealth()
{
	return MaxHealth == Health;
}


USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributesComp = GetAttributes(Actor);
	if(AttributesComp)
	{
		return AttributesComp->IsAlive();
	}
	return false;
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -MaxHealth);
}
