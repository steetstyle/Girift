// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Weapon/CoreWeapon.h"
#include "Blueprint/UserWidget.h"

#include "WeaponManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GIRIFT_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
    virtual void InitializeWeapons(void);
    virtual void RenewWeapons(void);

private:
    ACoreWeapon* CurrentWeapon;
    int32  CurrentWeaponIndex;
    TArray<ACoreWeapon*> EquableWeapons;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CoreSettings)
    TSubclassOf<ACoreWeapon> DefaultPrimaryWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CoreSettings)
    TSubclassOf<ACoreWeapon> DefaultSecondaryWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CoreSettings)
    TSubclassOf<ACoreWeapon> DefaultKnife;


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CoreSettings)
    TArray<ACoreWeapon*> AvailableWeapons;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CoreSettings)
    bool bRestartOrder;

public:
    virtual void ChangeWeapon_Next(void);
    virtual void ChangeWeapon_Previous(void);
    virtual void ChangeWeaponWith(ACoreWeapon* Weapon);
    virtual void DropCurrentWeapon(void);

public:
    virtual ACoreWeapon* GetOrCreateKnife(void);
    virtual ACoreWeapon* GetOrCreatePistol(void);
    virtual ACoreWeapon* GetOrCreatePrimary(void);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StoreSettings)
    TSubclassOf<class UUserWidget> StoreWidgetClass;
private:
    UUserWidget* StoreWidget;

public:
    virtual void Reload_Pressed(void);
    virtual void Reload_Released(void);

    virtual void InspectWeapon_Pressed(void);
    virtual void InspectWeapon_Released(void);

    virtual void HolsterWeapon_Pressed(void);
    virtual void HolsterWeapon_Released(void);

    virtual void ToggleLaserSight_Pressed(void);
    virtual void ToggleLaserSight_Released(void);

    virtual void ToggleFlashlight_Pressed(void);
    virtual void ToggleFlashlight_Released(void);

    virtual void LeftClick_Pressed(void);
    virtual void LeftClick_Released(void);

    virtual void RightClick_Pressed(void);
    virtual void RightClick_Released(void);

    virtual void Store_Pressed(void);
    virtual void Store_Released(void);
};
