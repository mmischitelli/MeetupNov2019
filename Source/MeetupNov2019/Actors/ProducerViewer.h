#pragma once

#include "GameFramework/Actor.h"
#include "ProducerViewer.generated.h"

UCLASS(Blueprintable)
class MEETUPNOV2019_API AProducerViewer : public AActor
{
	GENERATED_BODY()

	class UProducerSubsystem* m_ProducerSubsystem;
	TMap<FString, class UTextRenderComponent*> m_ProducersMap;
	
public:

	AProducerViewer();
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
};