#pragma once

#include "Components/SceneComponent.h"
#include "HorizontalLayoutComponent.generated.h"

UCLASS()
class MEETUPNOV2019_API UHorizontalLayoutComponent : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = HorizontalLayoutComponent, meta = (DisplayName = "Intra-element margin", AllowPrivateAccess = true, ClampMin="0", ClampMax="50"))
	float m_Margin = 10.0f;

public:
	UHorizontalLayoutComponent();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};