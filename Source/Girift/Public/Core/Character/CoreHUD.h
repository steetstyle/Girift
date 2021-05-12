// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core/Widgets/HUDWidget.h"
#include "Core/Weapon/CoreWeapon.h"
#include "UI/Widgets/HealthAmmoWidget.h"
#include "CoreHUD.generated.h"

/**
 * 
 */
UCLASS()
class GIRIFT_API ACoreHUD : public AHUD
{
    GENERATED_BODY()

public:

    ACoreHUD();
    virtual void DrawHUD() override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

public:
    UFUNCTION()
    void UpdateHealth(int32 value);
    UFUNCTION()
    void ResetHealth();

    UFUNCTION()
    void UpdateAmmo(const ACoreWeapon* Weapon);
    UFUNCTION()
    void ResetAmmo(const ACoreWeapon* Weapon);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
    TSubclassOf<class UUserWidget> HealthAmmoWidgetClass;

private:
    UHealthAmmoWidget* HealthAmmoWidget;

};