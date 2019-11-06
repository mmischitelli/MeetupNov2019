// Fill out your copyright notice in the Description page of Project Settings.


#include "Visualizer.h"
#include "Common/Utils.h"

// Sets default values
AVisualizer::AVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AVisualizer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		TArray<AActor*> Actors;
		GetAttachedActors(Actors);
		UStd::TForEach_N(Actors.CreateConstIterator(), Actors.Num(), [](AActor* actor) {
			actor->Destroy();
		});
	}
}
