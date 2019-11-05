// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Visualizer.generated.h"

/**
 * @class AVisualizer
 * @brief Represents a single paddock where crates are spawned
 */
UCLASS()
class MEETUPNOV2019_API AVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVisualizer();

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Spawns a new crate mid-air over this paddock */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnNewElement();
};
