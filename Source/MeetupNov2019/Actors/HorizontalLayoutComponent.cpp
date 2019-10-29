#include "HorizontalLayoutComponent.h"
#include "GameFramework/Actor.h"
#include "Common/Utils.h"

void UHorizontalLayoutComponent::OnChildAttached(USceneComponent* ChildComponent)
{
	Super::OnChildAttached(ChildComponent);
	RefreshPositions();
}

void UHorizontalLayoutComponent::OnChildDetached(USceneComponent* ChildComponent)
{
	Super::OnChildDetached(ChildComponent);
	RefreshPositions();
}

void UHorizontalLayoutComponent::RefreshPositions()
{
	const auto components = GetAttachChildren();
	if (components.Num() == 0)
		return;

	const auto Margin = 20;
	const auto TotalSize = UStd::TAccumulate_N(components.CreateConstIterator(), components.Num(), 0, [&](int curr, USceneComponent* Component) {
		return curr + Component->CalcLocalBounds().BoxExtent.Z + Margin;
	});
	const auto SingleSize = TotalSize / components.Num();
	const auto StartingOffset = -TotalSize / 2.0f;
	
	UStd::TForEach_N(components.CreateConstIterator(), components.Num(), [&, accumSize = 0](USceneComponent* Component) mutable
	{
		Component->SetRelativeLocation({ StartingOffset + accumSize, .0f, .0f});
		accumSize += Component->CalcLocalBounds().BoxExtent.Z + Margin;
	});
}
