// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if(ensure(MyController))
	{
		AActor* MyActor = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
		if(!MyActor)
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(MyActor);
		if (AttribComp)
		{
			if (AttribComp->IsMaxHealth())
			{
				return EBTNodeResult::Failed;
			}
			FCollisionQueryParams ColParams;
			FHitResult Res;
			ColParams.AddIgnoredActor(MyActor);
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();			
			if(BlackboardComp)
			{
				AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
				if(TargetActor)
				{
					GetWorld()->LineTraceSingleByChannel(Res, MyActor->GetActorLocation(), TargetActor->GetActorLocation(), ECC_Visibility, ColParams);
					if(Res.GetActor() == TargetActor)
					{
						return EBTNodeResult::Failed;
					}
				}				
			}
			

			AttribComp->ApplyHealthChange(MyActor, 100);
			return EBTNodeResult::Succeeded;
		}

	}	
	return EBTNodeResult::Failed;
}
