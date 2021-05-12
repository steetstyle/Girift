// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Widgets/HUDWidget.h"
#include "HealthAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class GIRIFT_API UHealthAmmoWidget : public UHUDWidget
{
	GENERATED_BODY()

public:
    UHealthAmmoWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;

public:
    void UpdateHealth(int32 value);
    void ResetHealth(void);

    void UpdateArmor(int32 value);
    void ResetArmor(void);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock* TXT_Health;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock* TXT_Armor;

public:
    void UpdateAmmo(const class ACoreWeapon* Weapon);
    void ResetAmmo(const class ACoreWeapon* Weapon);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock* TXT_RemainingAmmo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
    class UTextBlock* TXT_CurrentAmmo;


};
