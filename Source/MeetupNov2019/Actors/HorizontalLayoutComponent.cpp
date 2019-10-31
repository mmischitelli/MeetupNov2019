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

	TArray<AActor*> Actors;
	GetOwner()->GetAttachedActors(Actors);
	if (Actors.Num() == 0)
		return;

	const auto TotalSize = UStd::TAccumulate_N(Actors.CreateConstIterator(), Actors.Num(), .0f, [&](float curr, AActor* Actor) 
	{
		FVector Origin, Extent;
		Actor->GetActorBounds(true, Origin, Extent);
		return curr + Extent.Y * 2.0f + m_Margin;
	});
	const auto StartingOffset = (-TotalSize + m_Margin) * 0.5f;

	UStd::TForEach_N(Actors.CreateConstIterator(), Actors.Num(), [&, accumSize = .0f](AActor* Actor) mutable
	{
		FVector Origin, Extent;
		Actor->GetActorBounds(true, Origin, Extent);
		Actor->SetActorRelativeLocation({ StartingOffset + accumSize + Extent.Y + m_Margin * 0.5f, .0f, .0f });
		accumSize += Extent.Y * 2.0f + m_Margin;
	});
}
