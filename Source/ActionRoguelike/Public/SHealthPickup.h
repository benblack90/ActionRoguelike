// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupBase.h"
#include "SHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPickup : public ASPickupBase
{
	GENERATED_BODY()

public:
	ASHealthPickup();
protected:

	float HealValue;

	virtual void Deactivate() override;
	virtual void Respawn() override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
