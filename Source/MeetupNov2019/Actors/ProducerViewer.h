#pragma once

#include "GameFramework/Actor.h"
#include "ProducerViewer.generated.h"

class UHorizontalLayoutComponent;
struct FProducerInfo;

UCLASS(Blueprintable)
class MEETUPNOV2019_API AProducerViewer : public AActor
{
	GENERATED_BODY()

	class UProducerSubsystem* m_ProducerSubsystem;
	TMap<FString, class UTextRenderComponent*> m_ProducersMap;
	FTimerHandle m_StatsViewerTimerHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ProducerViewer", meta = (DisplayName = "Horizontal layout component", AllowPrivateAccess = true))
	UHorizontalLayoutComponent* m_LayoutComponent;
	
public:

	AProducerViewer(const FObjectInitializer& ObjectInitializer);
	void BeginPlay() override;

private:
	
	UTextRenderComponent* _CreateNewVisualizer();
	void _DestroyVisualizer(const FString& key);
	void _UpdateVisualizer(UTextRenderComponent* Component, const FProducerInfo& Data);
	
	void _Render();

};