// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PrinterSubsystem.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "PrinterSubsystem")
class MEETUPNOV2019_API UPrinterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintSetter = SetColor, BlueprintGetter = GetColor, Category = PrinterSubsystem, meta = (DisplayName="Color", AllowPrivateAccess=true))
	FColor m_Color = FColor::Yellow;
	UPROPERTY(EditAnywhere, BlueprintSetter = SetLifetime, BlueprintGetter = GetLifetime, Category = PrinterSubsystem, meta = (DisplayName = "Lifetime", AllowPrivateAccess = true))
	float m_Lifetime = 4.0f;
	
public:

	UFUNCTION(BlueprintCallable, Category = PrinterSubsystem)
	void PrintString(const FString& str) const;

	UFUNCTION(BlueprintCallable, Category = PrinterSubsystem)
	void SetColor(const FColor& color) { m_Color = color; }
	UFUNCTION(BlueprintCallable, Category = PrinterSubsystem)
	FColor GetColor() const { return m_Color; }

	UFUNCTION(BlueprintCallable, Category = PrinterSubsystem)
	void SetLifetime(float duration) { m_Lifetime = duration; }
	UFUNCTION(BlueprintCallable, Category = PrinterSubsystem)
	float GetLifetime() const { return m_Lifetime; }
};
