// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Core/Widgets/HUDWidget.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GIRIFT_API ACorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    ACorePlayerController();
    inline class UHUDWidget* GetHUDWidget() const { return HUDWidget; }


protected:
    UPROPERTY(Transient, BlueprintReadWrite, Category = "Girift Widgets")
    class UHUDWidget* HUDWidget;
};
