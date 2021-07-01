// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Weapon/CoreWeapon.h"
#include "Core/Character/CoreCharacter.h"
#include "Components/ArrowComponent.h"
#include "CoreAutomaticWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GIRIFT_API ACoreAutomaticWeapon : public ACoreWeapon
{
	GENERATED_BODY()

public:
    ACoreAutomaticWeapon();
    virtual void InitializeWeaponForCharacter(ACoreCharacter* Character) override;

public:


    UPROPERTY(EditAnywhere, Category = ArrowComponent)
    UArrowComponent* SpawnPoint_SilencerMuzzleFlashPosition;
	
};
