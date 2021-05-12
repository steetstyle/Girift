// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class GIRIFT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;


};
