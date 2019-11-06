#pragma once

#include "DataStructures.generated.h"

USTRUCT()
struct FProducerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "ProducerInfo")
	FString Name;
	UPROPERTY(EditDefaultsOnly, Category = "ProducerInfo")
	int NumValues;
};

USTRUCT()
struct FProducersStats
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "ProducerInfo")
	TArray<FProducerInfo> ProducersInfo;
};


