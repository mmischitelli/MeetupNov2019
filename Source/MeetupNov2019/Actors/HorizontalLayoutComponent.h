#pragma once

#include "Components/SceneComponent.h"
#include "HorizontalLayoutComponent.generated.h"

UCLASS()
class MEETUPNOV2019_API UHorizontalLayoutComponent : public USceneComponent
{
	GENERATED_BODY()
	
protected:
	void OnChildAttached(USceneComponent* ChildComponent) override;
	void OnChildDetached(USceneComponent* ChildComponent) override;
	
public:
	void RefreshPositions();
};