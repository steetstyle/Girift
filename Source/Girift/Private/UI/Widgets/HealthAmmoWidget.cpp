// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/HealthAmmoWidget.h"
#include "Core/Weapon/CoreWeapon.h"


UHealthAmmoWidget::UHealthAmmoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){

}

void UHealthAmmoWidget::NativeConstruct(){
    Super::NativeConstruct();
}

void UHealthAmmoWidget::UpdateHealth(int32 value){
    if(TXT_Health){
        TXT_Health->SetText(FText::FromString(FString::FromInt(value)));
    }
}

void UHealthAmmoWidget::ResetHealth(void){
    if(TXT_Health){
        TXT_Health->SetText(FText::FromString(FString::FromInt(0)));
    }
}

void UHealthAmmoWidget::UpdateArmor(int32 value){
    if(TXT_Armor){
        TXT_Armor->SetText(FText::FromString(FString::FromInt(value)));
    }
}

void UHealthAmmoWidget::ResetArmor(void){
    if(TXT_Armor){
        TXT_Armor->SetText(FText::FromString(FString::FromInt(0)));
    }
}

void UHealthAmmoWidget::UpdateAmmo(const ACoreWeapon* Weapon){
    if(Weapon != nullptr && TXT_RemainingAmmo && TXT_CurrentAmmo){

    }
}

void UHealthAmmoWidget::ResetAmmo(const ACoreWeapon* Weapon){
    if(Weapon != nullptr && TXT_RemainingAmmo && TXT_CurrentAmmo){

    }
}