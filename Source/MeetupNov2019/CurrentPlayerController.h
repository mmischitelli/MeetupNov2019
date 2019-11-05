#pragma once

#include "GameFramework/PlayerController.h"
#include "Actors/ProducerViewer.h"
#include "ThreadSafeBool.h"
#include "CurrentPlayerController.generated.h"


UCLASS()
class MEETUPNOV2019_API ACurrentPlayerController : public APlayerController
{
	GENERATED_BODY()

	// Handy pointer to the printer subsystem
	class UPrinterSubsystem* m_Printer;
	// Handy pointer to the producer subsystem
	class UProducerSubsystem* m_Producer;
	// Handy pointer to the actor that graphically represents what's happeing to the producers
	AProducerViewer* m_ProducerViewer;
	// Thread safe flag used to stop the consumer when there are no values to get
	FThreadSafeBool m_WaitForAsyncTask;
	// Accumulator for the values got from the producers
	double m_Sum;
	// Handle to the timer used to consume numbers from the producers
	FTimerHandle m_LongTickTimer;

	// Pointer to the class used to spawn the producer viewer actor
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (DisplayName = "Producer viewer class", AllowPrivateAccess = true))
		TSubclassOf<AProducerViewer> m_ProducerViewerClass;

protected:
	ACurrentPlayerController();

	//~ Begin AController interface
	/** Spawns and initializes the PlayerState for this Controller */
	void InitPlayerState() override;
	//~ End AController interface

	//~ Begin APlayerController interface
	/** Allows the PlayerController to set up custom input bindings. */
	void SetupInputComponent() override;
	//~ End APlayerController interface

private:
	/** Spawns a new producer thread */
	void _SpawnNewProducer();
	/** Kills the last producer thread created */
	void _KillProducer();
	/** Consumes a random value from the producers if there are any available.
	 * If all the producers are empty, makes an async request to get a new one then goes to sleep.
	 * Resumes when the value has been returned.
	 */
	void _ConsumeRandomValue();
	/** Callback function used to get a new random value asynchronously */
	UFUNCTION()
	void _GotRandomValue(float value);
};