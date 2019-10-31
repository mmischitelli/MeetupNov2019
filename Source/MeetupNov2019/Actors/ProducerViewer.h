#pragma once

#include "GameFramework/Actor.h"
#include "ProducerViewer.generated.h"

class AVisualizer;
class UHorizontalLayoutComponent;
struct FProducerInfo;

UCLASS(Blueprintable)
class MEETUPNOV2019_API AProducerViewer : public AActor
{
	GENERATED_BODY()

	class UProducerSubsystem* m_ProducerSubsystem;
	TMap<FString, AVisualizer*> m_ProducersMap;
	FTimerHandle m_LongTickTimer;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ProducerViewer", meta = (DisplayName = "Horizontal layout component", AllowPrivateAccess = true))
	UHorizontalLayoutComponent* m_LayoutComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProducerViewer", meta = (DisplayName = "Visualizer class", AllowPrivateAccess = true))
	TSubclassOf<AVisualizer> m_VisualizerClass;
	
public:

	AProducerViewer(const FObjectInitializer& ObjectInitializer);
	void BeginPlay() override;
	
private:

	// Creates new visualizers, removes unuseful ones and updates the rest.
	void _UpdateAll();

#pragma region CRUD Ops on visualizers
	/**
	 * Creates a new visualizer and attaches it to the layout component.
	 * Does not add it to m_ProducersMap!
	 * @return The newly created visualizer
	 */
	AVisualizer* _Create();
	/**
	 * Updates the visualizer's content.
	 * @param Visualizer The visualizer to update
	 * @param Data Content that will be displayed in this visualizer
	 */
	void _Update(AVisualizer* Visualizer, const FProducerInfo& Data);
	/**
	 * Destroys the visualizer identified by the provided key. For safety reasons, also removes the entry in m_ProducersMap.
	 * @param Key The key in m_ProducersMap corresponding to the visualizer to be deleted
	 */
	void _Destroy(const FString& Key);
#pragma endregion
};