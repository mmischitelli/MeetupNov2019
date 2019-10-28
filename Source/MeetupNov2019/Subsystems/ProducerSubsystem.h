// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Common\Utils.h"
#include "Threading\ProducerThread.h"
#include "Common/DataStructures.h"

#include "ProducerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRandomValueReadyDelegate, float, randomValue);

/**
 * 
 */
UCLASS(DisplayName = "ProducerSubsystem")
class MEETUPNOV2019_API UProducerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UDataStructs::TRange<int> m_Limits = UDataStructs::TRange<int>{ 1,5 };

	using ProducerThreadPtr = TUniquePtr<ProducerThread>;
	TArray<ProducerThreadPtr> m_Producers;

	FRandomValueReadyDelegate m_RandomValueReady;
	FThreadSafeBool m_PendingKill;
	
public:

	//~ Begin USubsystem Interface.
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	//~ End USubsystem Interface.

	FRandomValueReadyDelegate& OnRandomValueReady() { return m_RandomValueReady; }

	UFUNCTION(BlueprintCallable, Category = "ConsumerSubsystem|BusinessLogic")
	bool AddNewProducer();
	UFUNCTION(BlueprintCallable, Category = "ConsumerSubsystem|BusinessLogic")
	bool RemoveProducer();

	UFUNCTION(BlueprintCallable, Category = "ConsumerSubsystem|BusinessLogic")
	void GetRandomValueAsync();
	
	TOptional<float> TryGetRandomValue() const;

	UFUNCTION(BlueprintCallable, Category = "ConsumerSubsystem|Debug")
	void PrintStats() const;

	FProducersStats GetStats() const;
};
