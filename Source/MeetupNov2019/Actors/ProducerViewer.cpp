#include "ProducerViewer.h"
#include "Engine/GameInstance.h"
#include "..\Subsystems\ProducerSubsystem.h"
#include "Components/TextRenderComponent.h"

AProducerViewer::AProducerViewer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProducerViewer::BeginPlay()
{
	Super::BeginPlay();
	
	m_ProducerSubsystem = GetGameInstance()->GetSubsystem<UProducerSubsystem>();
}

void AProducerViewer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	const auto stats = m_ProducerSubsystem->GetStats();

	const auto kSize = 100.0f;
	const auto kStartingOffset = -(stats.ProducersInfo.Num() / 2.0f) * kSize;

	UStd::TForEach_N(stats.ProducersInfo.CreateConstIterator(), stats.ProducersInfo.Num(), [&, i = 0](const FProducerInfo& info) mutable 
	{
		auto component = m_ProducersMap.Find(info.Name);
		if (component == nullptr) 
		{
			component = &m_ProducersMap.Add(info.Name, NewObject<UTextRenderComponent>(this));
			(*component)->SetTextRenderColor(FColor::FromHex("FF2800"));
		}
		(*component)->SetText(FText::FromString(FString::FromInt(info.NumValues)));
		(*component)->SetRelativeLocation({ kStartingOffset + (kSize * i++) + kSize * .5f, .0f, .0f });
	});
}
