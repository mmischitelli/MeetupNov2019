#include "HorizontalLayoutComponent.h"
#include "GameFramework/Actor.h"
#include "Common/Utils.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

UHorizontalLayoutComponent::UHorizontalLayoutComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHorizontalLayoutComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const auto components = GetAttachChildren();
	if (components.Num() == 0)
		return;

	const auto TotalSize = UStd::TAccumulate_N(components.CreateConstIterator(), components.Num(), .0f, [&](float curr, USceneComponent* Component) {
		return curr + Component->CalcLocalBounds().BoxExtent.Y * 2.0f + m_Margin;
		});
	const auto StartingOffset = (-TotalSize + m_Margin) * 0.5f;

	UStd::TForEach_N(components.CreateConstIterator(), components.Num(), [&, accumSize = .0f](USceneComponent* Component) mutable
	{
		Component->SetRelativeLocation({ StartingOffset + accumSize, .0f, .0f });
		accumSize += Component->CalcLocalBounds().BoxExtent.Y * 2.0f + m_Margin;
	});
}
