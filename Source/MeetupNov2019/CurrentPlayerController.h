#pragma once

#include "GameFramework/PlayerController.h"
#include "Actors/ProducerViewer.h"
#include "CurrentPlayerController.generated.h"


UCLASS()
class MEETUPNOV2019_API ACurrentPlayerController : public APlayerController
{
	GENERATED_BODY()

	class UPrinterSubsystem* m_Printer;
	class UProducerSubsystem* m_Producer;
	AProducerViewer* m_ProducerViewer;
	
	bool m_WaitForAsyncTask;
	double m_Sum;

	FTimerHandle m_LongTickTimer;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (DisplayName="Producer viewer class", AllowPrivateAccess = true))
	TSubclassOf<AProducerViewer> m_ProducerViewerClass;

protected:
	ACurrentPlayerController();
	void InitPlayerState() override;
	void SetupInputComponent() override;
		
private:
	void _SpawnNewProducer();
	void _KillProducer();

	void _ConsumeCrates();

	UFUNCTION()
	void _GotRandomValue(float value);
};