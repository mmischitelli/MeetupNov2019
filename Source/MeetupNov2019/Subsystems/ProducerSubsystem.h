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
 * @class UProducerSubsystem
 * @brief Producers manager object that acts as middleware between producer threads and the consumer
 */
UCLASS(DisplayName = "ProducerSubsystem")
class MEETUPNOV2019_API UProducerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	using ProducerThreadPtr = TUniquePtr<ProducerThread>;

	/** Handy pointer to the printer subsystem */
	UPROPERTY()
	class UPrinterSubsystem* m_Printer;

	/** Storage for the allowed range of producers to have at any time */
	UDataStructs::TRange<int> m_Limits = UDataStructs::TRange<int>{ 1,5 };

	/** Array of producer threads */
	TArray<ProducerThreadPtr> m_Producers;

	/** Random value ready delegate */
	FRandomValueReadyDelegate m_RandomValueReady;
	/** Thread safe flag to gracefully die when the game is closing */
	FThreadSafeBool m_PendingKill;
	/** Unreal's PRNG facility */
	FRandomStream m_Random;
	
public:

	//~ Begin USubsystem Interface.
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	//~ End USubsystem Interface.

	/** On random value ready delegate to bind to be notified when a new random value becomes available */
	FRandomValueReadyDelegate& OnRandomValueReady() { return m_RandomValueReady; }

	/** Adds a new producer thread to the pool
	 * @return True if a new producer was spawned, false otherwise (reached the upper limit)
	 */
	UFUNCTION(BlueprintCallable, Category = "ConsumerSubsystem|BusinessLogic")
	bool AddNewProducer();
	/** Kills a producer thread and removes it from the pool
	 * @return True if a producer was removed, false otherwise (reached the lower limit)
	 */
	UFUNCTION(BlueprintCallable, Category = "ConsumerSubsystem|BusinessLogic")
	bool RemoveProducer();
	/** Asks this producer to provide a new random value asynchronously.
	 * It's slower than the other, direct method, so just use it when there are no more numbers available.
	 */
	UFUNCTION(BlueprintCallable, Category = "ConsumerSubsystem|BusinessLogic")
	void GetRandomValueAsync();
	/** Returns a random number from a random producer
	 * @return An optional which might contain a new random number if any producer has still some left. Unset if no numbers are available.
	 */
	TOptional<float> TryGetRandomValue() const;
	/** Prints some stats about the current status of the producers
	 * Similar to what is shown by the producer viewer actor
	 */
	UFUNCTION(BlueprintCallable, Category = "ConsumerSubsystem|Debug")
	void PrintStats() const;
	/** Gets the current status of the producers
	 * @return A struct that holds the current status of the producers
	 */
	FProducersStats GetStats() const;
};
