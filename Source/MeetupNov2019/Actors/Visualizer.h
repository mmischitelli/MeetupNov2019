// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Visualizer.generated.h"

UCLASS()
class MEETUPNOV2019_API AVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVisualizer();

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnNewElement();
};
