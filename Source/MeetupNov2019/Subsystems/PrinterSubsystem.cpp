// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinterSubsystem.h"
#include "Engine/Engine.h"

void UPrinterSubsystem::PrintString(const FString& str) const
{
	GEngine->AddOnScreenDebugMessage(-1, m_Lifetime, m_Color, str);
}
