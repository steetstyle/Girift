// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapon/Automatic/CoreAutomaticWeapon.h"


ACoreAutomaticWeapon::ACoreAutomaticWeapon() : ACoreWeapon() {

    FVector Location_PointLight_MuzzleFlashLight = FVector(-17.0f, 148.f, 165.f);
    FRotator Rotator_PointLight_MuzzleFlashLight = FRotator(0.3f, 0.4f, 90.0f);

    PointLight_MuzzleFlashLight->SetRelativeLocation(Location_PointLight_MuzzleFlashLight);
    PointLight_MuzzleFlashLight->SetRelativeRotation(Rotator_PointLight_MuzzleFlashLight);

    FVector Location_SpawnPoint_MuzzleFlashPosition = FVector(-27.0f, 110.f, 109.f);
    FRotator Rotator_SpawnPoint_MuzzleFlashPosition = FRotator(0.3f, 0.4f, 90.0f);

    SpawnPoint_MuzzleFlashPosition->SetRelativeLocation(Location_SpawnPoint_MuzzleFlashPosition);
    SpawnPoint_MuzzleFlashPosition->SetRelativeRotation(Rotator_SpawnPoint_MuzzleFlashPosition);

    SpawnPoint_Casing = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_Casing"));
    SpawnPoint_Casing->SetupAttachment(UC_WeaponComponents);

    SpawnPoint_MuzzleFlashSilencerPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_MuzzleFlashSilencerPosition"));
    SpawnPoint_MuzzleFlashSilencerPosition->SetupAttachment(UC_WeaponComponents);

    FVector Scale_BC_WeaponCollider = FVector(24.0f, 3.0f, 5.0f);

    BC_WeaponCollider->SetRelativeScale3D(Scale_BC_WeaponCollider);

}

void ACoreAutomaticWeapon::InitializeWeaponForCharacter(ACoreCharacter* Character){
    Super::InitializeWeaponForCharacter(Character);
}
