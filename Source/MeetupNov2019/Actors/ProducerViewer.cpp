#include "ProducerViewer.h"
#include "Engine/GameInstance.h"
#include "..\Subsystems\ProducerSubsystem.h"
#include "TimerManager.h"
#include "HorizontalLayoutComponent.h"
#include "Async.h"
#include "Visualizer.h"
#include "Components/StaticMeshComponent.h"

AProducerViewer::AProducerViewer(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	m_LayoutComponent = CreateDefaultSubobject<UHorizontalLayoutComponent>(TEXT("LayoutComponent"));
	m_LayoutComponent->SetupAttachment(SceneComponent);
}

void AProducerViewer::BeginPlay()
{
	Super::BeginPlay();
	
	m_ProducerSubsystem = GetGameInstance()->GetSubsystem<UProducerSubsystem>();

	GetWorldTimerManager().SetTimer(m_LongTickTimer, this, &AProducerViewer::_UpdateAll, 0.2f, true);
}

void AProducerViewer::_UpdateAll()
{
	// Create new visualizers and update the rest
	const auto Stats = m_ProducerSubsystem->GetStats();
	UStd::TForEach_N(Stats.ProducersInfo.CreateConstIterator(), Stats.ProducersInfo.Num(), [&, i = 0](const FProducerInfo& Info) mutable
	{
		auto Visualizer = m_ProducersMap.Find(Info.Name);
		if (Visualizer == nullptr) 
		{
			const auto visualizer = _Create();
			if (!visualizer)
				return;
			Visualizer = &m_ProducersMap.Add(Info.Name, visualizer);
		}

		_Update(*Visualizer, Info);
	});

	// Find deleted producers
	TArray<FString> m_MissingProducers;
	m_MissingProducers.SetNum(m_ProducersMap.Num());
	UStd::TTransform_N(m_ProducersMap.CreateConstIterator(), m_ProducersMap.Num(), m_MissingProducers.CreateIterator(), [&](const TTuple<FString, AVisualizer*>& iter)
	{
		const bool bIsPresentInStats = Stats.ProducersInfo.ContainsByPredicate([producerName = iter.Key](const FProducerInfo& info) { return info.Name == producerName; });
		return !bIsPresentInStats ? iter.Key : "";
	});
	m_MissingProducers.RemoveAllSwap([](const FString& elem) { return elem.IsEmpty(); });

	// Remove the visualizers corresponding to the deleted producers
	UStd::TForEach_N(m_MissingProducers.CreateConstIterator(), m_MissingProducers.Num(), [&](const FString& str) {
		_Destroy(str);
	});
}

#pragma region CRUD Ops on visualizers

AVisualizer* AProducerViewer::_Create()
{
	if (m_VisualizerClass)
	{
		auto visualizer = GetWorld()->SpawnActor<AVisualizer>(m_VisualizerClass);
		visualizer->SetActorRelativeRotation(FRotator({ 0,0,1,0 }));
		visualizer->AttachToComponent(m_LayoutComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, false));
		return visualizer;
	}

	/*
	auto Component = NewObject<UTextRenderComponent>(this);
	Component->RegisterComponent();
	Component->SetRelativeRotation(FRotator({ 0,0,1,0 }));
	Component->AttachToComponent(m_LayoutComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, false));
	Component->SetTextRenderColor(FColor::FromHex("FF2800"));
	*/

	return nullptr;
}

void AProducerViewer::_Update(AVisualizer* Visualizer, const FProducerInfo& Data)
{
	TArray<AActor*> crates;
	Visualizer->GetAttachedActors(crates);
	if (crates.Num() > Data.NumValues)
	{
		UStd::TForEach_N(crates.CreateIterator(), crates.Num() - Data.NumValues, [](AActor* actorComp) {
			actorComp->Destroy();
		});
		return;
	}
	
	for (int i = 0; i < Data.NumValues - crates.Num(); ++i) {
		Visualizer->SpawnNewElement();
	}
	
	//Component->SetText(FText::FromString(FString::FromInt(Data.NumValues)));
}

void AProducerViewer::_Destroy(const FString& Key)
{
	const auto Producer = m_ProducersMap.Find(Key);
	if (Producer != nullptr)
	{
		(*Producer)->Destroy();// DestroyComponent();
		m_ProducersMap.Remove(Key);		
	}
}

#pragma endregion
