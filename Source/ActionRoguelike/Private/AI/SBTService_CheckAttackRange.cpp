// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//check distance between ai entity and target actor
	//Line of sight checks

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if(ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if(ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithinRange = DistanceTo < 2000.0f;
					bool bHasLoS = false;
					if(bWithinRange)
					{
						//line of sight check built into AI module
						bHasLoS = MyController->LineOfSightTo(TargetActor);
					}
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLoS);
				}
			}
		}
	}
}