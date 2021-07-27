// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/Components/WeaponManagerComponent.h"
#include "Core/Weapon/Pistols/CorePistol.h"
#include "Core/Weapon/Automatic/CoreAutomaticWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Character/CoreCharacter.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentWeapon = nullptr;
	CurrentWeaponIndex = 0;
	AvailableWeapons.Empty();
	EquableWeapons.Empty();
	bRestartOrder = true;

}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
    InitializeWeapons();
}

// Called every frame
void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// Called to bind functionality to input
void UWeaponManagerComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

    PlayerInputComponent->BindAction("InspectWeapon", IE_Pressed, this, &UWeaponManagerComponent::InspectWeapon_Pressed);
    PlayerInputComponent->BindAction("InspectWeapon", IE_Released, this, &UWeaponManagerComponent::InspectWeapon_Released);
    PlayerInputComponent->BindAction("HolsterWeapon", IE_Pressed, this, &UWeaponManagerComponent::HolsterWeapon_Pressed);
    PlayerInputComponent->BindAction("HolsterWeapon", IE_Released, this, &UWeaponManagerComponent::HolsterWeapon_Released);
    PlayerInputComponent->BindAction("ToggleFlashlight", IE_Pressed, this, &UWeaponManagerComponent::ToggleFlashlight_Pressed);
    PlayerInputComponent->BindAction("ToggleFlashlight", IE_Released, this, &UWeaponManagerComponent::ToggleFlashlight_Released);

    PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &UWeaponManagerComponent::LeftClick_Pressed);
    PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &UWeaponManagerComponent::LeftClick_Released);
    PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &UWeaponManagerComponent::RightClick_Pressed);
    PlayerInputComponent->BindAction("RightClick", IE_Released, this, &UWeaponManagerComponent::RightClick_Released);

    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &UWeaponManagerComponent::Reload_Pressed);
    PlayerInputComponent->BindAction("Reload", IE_Released, this, &UWeaponManagerComponent::Reload_Released);

    PlayerInputComponent->BindAction("Store", IE_Pressed, this, &UWeaponManagerComponent::Store_Pressed);
    PlayerInputComponent->BindAction("Store", IE_Released, this, &UWeaponManagerComponent::Store_Released);

    PlayerInputComponent->BindAction("ChangeWeapon_Next", IE_Pressed, this, &UWeaponManagerComponent::ChangeWeapon_Next);
    PlayerInputComponent->BindAction("ChangeWeapon_Previous", IE_Pressed, this, &UWeaponManagerComponent::ChangeWeapon_Previous);

}

void UWeaponManagerComponent::InitializeWeapons(void)
{
    APawn* CorePawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);

   if(CorePawn){
       if(DefaultPrimaryWeapon){

           FActorSpawnParameters SpawnInfo;
           SpawnInfo.Instigator = CorePawn;

           FVector Location = FVector(0.f, 0.f, 0.f);
           FRotator Rotator = FRotator(0.f, 0.f, 0.f);

           AActor* SpawnedActor = GetWorld()->SpawnActor(DefaultPrimaryWeapon, &Location, &Rotator, SpawnInfo);
           ACoreAutomaticWeapon* CharacterPrimaryGun = Cast<ACoreAutomaticWeapon>(SpawnedActor);

           if(CharacterPrimaryGun){
               AvailableWeapons.Add(CharacterPrimaryGun);
           }
       }


       if(DefaultSecondaryWeapon){
           FActorSpawnParameters SpawnInfo;
           SpawnInfo.Instigator = CorePawn;

           FVector Location = FVector(0.f, 0.f, 0.f);
           FRotator Rotator = FRotator(0.f, 0.f, 0.f);

           AActor* SpawnedActor = GetWorld()->SpawnActor(DefaultSecondaryWeapon, &Location, &Rotator, SpawnInfo);
           ACorePistol* CharacterSecondaryGun = Cast<ACorePistol>(SpawnedActor);

           if(CharacterSecondaryGun){
               AvailableWeapons.Add(CharacterSecondaryGun);
           }
       }

       if(DefaultKnife){

       }

       for (int j = 0; j < AvailableWeapons.Num(); ++j) {
           if(AvailableWeapons[j]){
               CurrentWeaponIndex = j;
               CurrentWeapon = AvailableWeapons[j];
               ACoreCharacter* CoreCharacter = Cast<ACoreCharacter>(CorePawn);
               CurrentWeapon->InitializeWeaponForCharacter(CoreCharacter);
               return;
           }
       }


   }
}

ACoreWeapon* UWeaponManagerComponent::GetCurrentWeapon(void)
{
    return CurrentWeapon;
}


void UWeaponManagerComponent::ChangeWeapon_Next(void){
    UE_LOG(LogTemp, Warning, TEXT("ChangeWeapon_Next Called"));
    int32 Count = AvailableWeapons.Num();

    if(Count < 1){
        return;
    }

    if(CurrentWeaponIndex + 1 >= Count){
        if(bRestartOrder){
            CurrentWeaponIndex = 0;
        }
        else{
            return;
        }
    }
    else if(CurrentWeaponIndex + 1 < Count){
        CurrentWeaponIndex = CurrentWeaponIndex + 1;
    }
    else{
        // Character Have no weapon only knife
        return;
    }

    if(CurrentWeapon){
        CurrentWeapon->DeinitializeWeapon();
    }

    CurrentWeapon = AvailableWeapons[CurrentWeaponIndex];
    if(CurrentWeapon){
        APawn* CorePawn = UGameplayStatics::GetPlayerCharacter(GetOwner(),0);
        ACoreCharacter* CoreCharacter = Cast<ACoreCharacter>(CorePawn);
        CurrentWeapon->InitializeWeaponForCharacter(CoreCharacter);
    }

}

void UWeaponManagerComponent::ChangeWeapon_Previous(void){
   UE_LOG(LogTemp, Warning, TEXT("ChangeWeapon_Previous Called"));

   int32 Count = AvailableWeapons.Num();

    if(Count < 1){
        return;
    }

    if(CurrentWeaponIndex - 1 < 0 ){
        if(bRestartOrder){
            CurrentWeaponIndex = Count - 1;
        }
        else{
            return;
        }
    }
    else if(CurrentWeaponIndex - 1 < Count){
        CurrentWeaponIndex = CurrentWeaponIndex - 1;
    }
    else{
        // Character Have no weapon only knife
        return;
    }

    if(CurrentWeapon){
        CurrentWeapon->DeinitializeWeapon();
    }

    CurrentWeapon = AvailableWeapons[CurrentWeaponIndex];
    if(CurrentWeapon){
        ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetOwner(),0);
        ACoreCharacter* CoreCharacter = Cast<ACoreCharacter>(Character);
        CurrentWeapon->InitializeWeaponForCharacter(CoreCharacter);
    }
}

void UWeaponManagerComponent::ChangeWeaponWith(ACoreWeapon* Weapon){
    if(Weapon != nullptr){
        AvailableWeapons[CurrentWeaponIndex] = Weapon;

        if(CurrentWeapon){
            CurrentWeapon->DeinitializeWeapon();
        }

        CurrentWeapon = Weapon;
        if(CurrentWeapon){
            ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetOwner(),0);
            ACoreCharacter* CoreCharacter = Cast<ACoreCharacter>(Character);
            CurrentWeapon->InitializeWeaponForCharacter(CoreCharacter);
        }
    }
}

void UWeaponManagerComponent::DropCurrentWeapon(void){
    CurrentWeapon->DeinitializeWeapon();
    AvailableWeapons[CurrentWeaponIndex] = nullptr;
    CurrentWeapon = nullptr;
}

void UWeaponManagerComponent::RenewWeapons(void){

    //TODO:: Renew all weapon properties Ammo, Remaining Ammo vs.
    for (ACoreWeapon* Weapon : AvailableWeapons)
    {
        if(Weapon != nullptr)  {
            Weapon->Reload();
        }
    }

}

ACoreWeapon* UWeaponManagerComponent::GetOrCreateKnife(void){
    ACoreWeapon* CharacterKnife = nullptr;

    //TODO:: Get the knife from character if getting null create default with knife skin

    return CharacterKnife;
}


ACoreWeapon* UWeaponManagerComponent::GetOrCreatePistol(void){

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Owner = GetOwner();

    const FVector Location = FVector(0.0f, 0.0f, 0.0f);
    const FRotator Rotator = FRotator(0.0f, 0.0f, 0.0f);

    ACoreWeapon* CharacterPistol = Cast<ACorePistol>(GetWorld()->SpawnActor(ACorePistol::StaticClass(), &Location, &Rotator, SpawnInfo));

    //TODO:: Get the pistol from character if getting null create default with pistol skin

    return CharacterPistol;
}

ACoreWeapon* UWeaponManagerComponent::GetOrCreatePrimary(void){

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Owner = GetOwner();

    const FVector Location = FVector(0.0f, 0.0f, 0.0f);
    const FRotator Rotator = FRotator(0.0f, 0.0f, 0.0f);

    ACoreWeapon* CharacterPrimaryWeapon = Cast<ACorePistol>(GetWorld()->SpawnActor(ACoreAutomaticWeapon::StaticClass(), &Location, &Rotator, SpawnInfo));

    //TODO:: Get the primary weapon from character if getting null create default with primary weapon skin

    return CharacterPrimaryWeapon;
}


void UWeaponManagerComponent::Reload_Pressed(void)
{
    if(CurrentWeapon){
        CurrentWeapon->Reload_Pressed();
    }
}

void UWeaponManagerComponent::Reload_Released(void)
{
    if(CurrentWeapon){
        CurrentWeapon->Reload_Released();
    }
}

void UWeaponManagerComponent::InspectWeapon_Pressed(void)
{
    if(CurrentWeapon){
        CurrentWeapon->InspectWeapon_Pressed();
    }
}

void UWeaponManagerComponent::InspectWeapon_Released(void)
{
    if(CurrentWeapon){
        CurrentWeapon->InspectWeapon_Released();
    }
}

void UWeaponManagerComponent::HolsterWeapon_Pressed(void)
{
    if(CurrentWeapon){
        CurrentWeapon->HolsterWeapon_Pressed();
    }
}

void UWeaponManagerComponent::HolsterWeapon_Released(void)
{
    if(CurrentWeapon){
        CurrentWeapon->HolsterWeapon_Released();
    }
}

void UWeaponManagerComponent::ToggleLaserSight_Pressed(void)
{
    if(CurrentWeapon){
        CurrentWeapon->ToggleLaserSight_Pressed();
    }
}

void UWeaponManagerComponent::ToggleLaserSight_Released(void)
{
    if(CurrentWeapon){
        CurrentWeapon->ToggleLaserSight_Released();
    }
}

void UWeaponManagerComponent::ToggleFlashlight_Pressed(void)
{
    if(CurrentWeapon){
        CurrentWeapon->ToggleFlashlight_Pressed();
    }
}

void UWeaponManagerComponent::ToggleFlashlight_Released(void)
{
    if(CurrentWeapon){
        CurrentWeapon->ToggleFlashlight_Released();
    }
}

void UWeaponManagerComponent::LeftClick_Pressed(void)
{
    if(CurrentWeapon){
        CurrentWeapon->LeftClick_Pressed();
    }
}

void UWeaponManagerComponent::LeftClick_Released(void)
{
    if(CurrentWeapon){
        CurrentWeapon->LeftClick_Released();
    }
}

void UWeaponManagerComponent::RightClick_Pressed(void)
{
    if(CurrentWeapon){
        CurrentWeapon->RightClick_Pressed();
    }
}

void UWeaponManagerComponent::RightClick_Released(void)
{
    if(CurrentWeapon){
        CurrentWeapon->RightClick_Released();
    }
}

void UWeaponManagerComponent::Store_Pressed(void)
{
    StoreWidget = CreateWidget<UUserWidget>(GetWorld(), StoreWidgetClass);

    if (StoreWidget)
    {
        StoreWidget->AddToViewport();
    }

}

void UWeaponManagerComponent::Store_Released(void)
{
    if (StoreWidget)
    {
        StoreWidget->RemoveFromParent();
    }
}