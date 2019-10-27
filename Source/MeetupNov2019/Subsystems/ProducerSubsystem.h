// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Common\Utils.h"
#include "Threading\ProducerThread.h"

#include "ProducerSubsystem.generated.h"

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
	
public:

	//~ Begin USubsystem Interface.
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	//~ End USubsystem Interface.

	UFUNCTION(BlueprintCallable, Category = ConsumerSubsystem)
	bool AddNewProducer();
	UFUNCTION(BlueprintCallable, Category = ConsumerSubsystem)
	bool RemoveProducer();

	UFUNCTION(BlueprintCallable, Category = ConsumerSubsystem)
	void PrintStats() const;
};
