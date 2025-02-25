// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
	:LowHealthThreshold{0.33f}
{
	
}


void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AAIController* OwnerController = OwnerComp.GetAIOwner();
		
		if(ensure(OwnerController))
		{
			AActor* OwnerActor = Cast<AActor>(OwnerController->GetPawn());
			if (ensure(OwnerActor))
			{
				USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(OwnerActor);
				if (AttribComp)
				{
					bool bLowHealth = AttribComp->GetHealthAsFraction() < LowHealthThreshold;
					if(bLowHealth)
					{
						DrawDebugString(GetWorld(), OwnerActor->GetActorLocation(), "LowHealth!!", nullptr, FColor::Cyan, 4.0, true);
					}
					BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
				}
			}
		}
		
	}
}
