// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageDealer, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttribComp)
	{
		return AttribComp->ApplyHealthChange(DamageDealer, DamageAmount);
	}
	return false;
}

//fun bit of blueprint magic here - passing a parameter as a reference actually means it will be put as an 'output pin' on the blueprint node
//to avoid this, pass a const reference, as per the FHitResult& here. Now it's a reference that does not get put as an output pin ... because it's const so can't be changed
bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageDealer, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageDealer, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 30000.0f, HitResult.ImpactPoint, HitResult.BoneName);
			return true;
		}
	}
	return false;
}
