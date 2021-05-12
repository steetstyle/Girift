// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/CoreHUD.h"

ACoreHUD::ACoreHUD(){

}

void ACoreHUD::DrawHUD(){
    Super::DrawHUD();

}

void ACoreHUD::BeginPlay(){
    Super::BeginPlay();
    if(HealthAmmoWidgetClass){
        HealthAmmoWidget = CreateWidget<UHealthAmmoWidget>(GetWorld(), HealthAmmoWidgetClass);
        if(HealthAmmoWidget){
            HealthAmmoWidget->AddToViewport();
        }
    }

}

void ACoreHUD::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
}

void ACoreHUD::UpdateHealth(int32 value){
    if(HealthAmmoWidget){
        HealthAmmoWidget->UpdateHealth(value);
    }
}

void ACoreHUD::ResetHealth(){
    if(HealthAmmoWidget){
        HealthAmmoWidget->ResetHealth();
    }
}

void ACoreHUD::UpdateAmmo(const ACoreWeapon* Weapon){
    if(HealthAmmoWidget){
        HealthAmmoWidget->UpdateAmmo(Weapon);
    }
}

void ACoreHUD::ResetAmmo(const ACoreWeapon* Weapon){
    if(HealthAmmoWidget){
        HealthAmmoWidget->ResetAmmo(Weapon);
    }
}