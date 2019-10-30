#include "ProducerViewer.h"
#include "Engine/GameInstance.h"
#include "..\Subsystems\ProducerSubsystem.h"
#include "Components/TextRenderComponent.h"
#include "TimerManager.h"
#include "HorizontalLayoutComponent.h"
#include "Async.h"
#include "Subsystems/PrinterSubsystem.h"

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
		auto Component = m_ProducersMap.Find(Info.Name);
		if (Component == nullptr) {
			Component = &m_ProducersMap.Add(Info.Name, _Create());
		}

		_Update(Cast<UTextRenderComponent>(*Component), Info);
	});

	// Find deleted producers
	TArray<FString> m_MissingProducers;
	m_MissingProducers.SetNum(m_ProducersMap.Num());
	UStd::TTransform_N(m_ProducersMap.CreateConstIterator(), m_ProducersMap.Num(), m_MissingProducers.CreateIterator(), [&](const TTuple<FString, UTextRenderComponent*>& iter)
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

UTextRenderComponent* AProducerViewer::_Create()
{
	auto Component = NewObject<UTextRenderComponent>(this);
	Component->RegisterComponent();
	Component->SetRelativeRotation(FRotator({ 0,0,1,0 }));
	Component->AttachToComponent(m_LayoutComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, false));
	Component->SetTextRenderColor(FColor::FromHex("FF2800"));
	return Component;
}

void AProducerViewer::_Update(UTextRenderComponent* Component, const FProducerInfo& Data)
{
	Component->SetText(FText::FromString(FString::FromInt(Data.NumValues)));
}

void AProducerViewer::_Destroy(const FString& Key)
{
	const auto Producer = m_ProducersMap.Find(Key);
	if (Producer != nullptr)
	{
		(*Producer)->DestroyComponent();
		m_ProducersMap.Remove(Key);		
	}
}

#pragma endregion
