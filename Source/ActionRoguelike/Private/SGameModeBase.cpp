// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EngineUtils.h"
#include <SCharacter.h>


//this is a console variable. This allows us to change something at run-time in game - in this case, this is a bool to signal bot spawning to stop.
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning a bot via a timer"),ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
	:SpawnTimerInterval{2.0f}
{
	
}

void ASGameModeBase::StartPlay()
{
	//we better call super, cuz this superclass version of this function calls BeginPlay!!
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotsTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotsTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning has been disabled via CVarSpawnBots"));
		return;
	};
	int32 NumAliveBots = 0;

	//The TActorIterator<...> returns all classes specified. The UWorld is passed into the constructor, so it knows which world to get the classes from.
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttribComp) && AttribComp->IsAlive())
		{
			NumAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NumAliveBots);

	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);

	}

	if (NumAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At max bot capacity; skipping bot spawn"));
		return;
	}

	//this function is designed to be used with blueprints, so returns a UEnvQueryInstanceBlueprintWrapper pointer, so we'll have to work around that!
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}	
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnBot EQS Query Failed!"));
		return;
	}	

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}



void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttribComp) && AttribComp->IsAlive())
		{
			AttribComp->Kill(this); //@fixme: pass in player for kill credit in future??
		}
	}
}

void ASGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(Victim);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay,false);
		UE_LOG(LogTemp, Log, TEXT("OnActorKilled - Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}
