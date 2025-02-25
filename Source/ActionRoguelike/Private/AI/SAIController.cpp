// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("Please assign a behaviour tree in your ai controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	

	//APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (ensureAlways(Player))
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", Player->GetActorLocation());

	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", Player);
	//}

}