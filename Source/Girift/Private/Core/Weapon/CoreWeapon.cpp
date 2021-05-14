// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapon/CoreWeapon.h"
#include "Core/Character/CoreCharacter.h"

// Sets default values
ACoreWeapon::ACoreWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PSN_Weapon = TEXT("Weapon_Position");
    PSN_Magazine = TEXT("Mag_Position");
    PSN_Slider = TEXT("Slider_Position");

    UC_Root = CreateDefaultSubobject<USceneComponent>(TEXT("UC_Root"));
    UC_Root->SetupAttachment(GetRootComponent());

    UC_WeaponComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_WeaponComponents"));
    UC_WeaponComponents->SetupAttachment(GetRootComponent(), PSN_Weapon);

    SM_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Weapon"));
    SM_Weapon->SetupAttachment(UC_WeaponComponents);

    BC_WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BC_WeaponCollider"));

    FVector BoxExtent_BC_WeaponCollider = FVector(1.0f, 1.0f, 1.0f);
    BC_WeaponCollider->SetBoxExtent(BoxExtent_BC_WeaponCollider,true);

    BC_WeaponCollider->SetupAttachment(UC_WeaponComponents);

    PointLight_MuzzleFlashLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight_MuzzleFlashLight"));
    PointLight_MuzzleFlashLight->SetupAttachment(UC_WeaponComponents);

    SpawnPoint_MuzzleFlashPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_MuzzleFlashPosition"));
    SpawnPoint_MuzzleFlashPosition->SetupAttachment(UC_WeaponComponents);

    UC_MagazineComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_MagazineComponents"));
    UC_MagazineComponents->SetupAttachment(GetRootComponent(), PSN_Magazine);

    SM_Mag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Mag"));
    SM_Mag->SetupAttachment(UC_MagazineComponents);

    SM_Bullets = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Bullets"));
    SM_Bullets->SetupAttachment(UC_MagazineComponents);

    UC_SliderComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_SliderComponents"));
    UC_SliderComponents->SetupAttachment(GetRootComponent(), PSN_Slider);

    SM_Slider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Slider"));
    SM_Slider->SetupAttachment(UC_SliderComponents);


    isOutOfAmmo = false;
    isReloadingOutOfAmmo = false;
    isReloadingAmmoLeft = false;

    totalAmmo = 75;
    currentAmmo = 25;
    maxCurrentAmmo = 25;

    FireRate = 0.1f;

    WeaponFireModeStatus = EWeaponFireModeStatus::None;
}

// Called when the game starts or when spawned
void ACoreWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACoreWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoreWeapon::InitializeWeaponForCharacter(ACoreCharacter* Character)
{
    UE_LOG(LogTemp, Warning, TEXT("InitializeWeaponForCharacter Called"));

    OwnerCharacter = Character;
    if(OwnerCharacter){
        UC_WeaponComponents->AttachToComponent(OwnerCharacter->GetArmsHolderSkeletalMesh(), FAttachmentTransformRules::KeepRelativeTransform, PSN_Weapon);
        UC_MagazineComponents->AttachToComponent(OwnerCharacter->GetArmsHolderSkeletalMesh(), FAttachmentTransformRules::KeepRelativeTransform, PSN_Magazine);
        UC_SliderComponents->AttachToComponent(OwnerCharacter->GetArmsHolderSkeletalMesh(), FAttachmentTransformRules::KeepRelativeTransform, PSN_Slider);
        UE_LOG(LogTemp, Warning, TEXT("Weapon Attached to OwnerCharacter Skeletal Component"));

        if(WeaponAnimClass){
            OwnerCharacter->GetArmsHolderSkeletalMesh()->SetAnimInstanceClass(WeaponAnimClass);
        }
    }
    else{
        UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter Not Derived on CoreCharacter or nullptr"));
    }
}

void ACoreWeapon::DeinitializeWeapon()
{
    UE_LOG(LogTemp, Warning, TEXT("DeinitializeWeapon Called"));
    if(OwnerCharacter){
        UC_WeaponComponents->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        UC_MagazineComponents->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        UC_SliderComponents->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        UE_LOG(LogTemp, Warning, TEXT("Weapon Detached from OwnerCharacter"));
    }
    else{
        UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter Not Derived on CoreCharacter or nullptr"));
    }
}

void ACoreWeapon::Reload_Pressed(void)
{

}

void ACoreWeapon::Reload_Released(void)
{

}

void ACoreWeapon::InspectWeapon_Pressed(void)
{

}

void ACoreWeapon::InspectWeapon_Released(void)
{

}

void ACoreWeapon::HolsterWeapon_Pressed(void)
{

}

void ACoreWeapon::HolsterWeapon_Released(void)
{

}

void ACoreWeapon::ToggleLaserSight_Pressed(void)
{

}

void ACoreWeapon::ToggleLaserSight_Released(void)
{

}

void ACoreWeapon::ToggleFlashlight_Pressed(void)
{

}

void ACoreWeapon::ToggleFlashlight_Released(void)
{

}

void ACoreWeapon::LeftClick_Pressed(void)
{
    isLeftClickPressed = true;
	
    if (OwnerCharacter)
    {
        if (!(OwnerCharacter->IsWeaponCanFire() && !OwnerCharacter->IsWeaponShooting()) && OwnerCharacter->IsWeaponOutOfAmmo())
        {
            if(OwnerCharacter->IsAiming())
            {
	            // Play out of ammo animation (Aiming State)
            }
            else
            {
                // Play out of ammo animation (Normal State)

            }
        }

    	if(!OwnerCharacter->IsWeaponCanFire())
    	{
            // Disable Shooting if any other actions are active
            return;
    	}

        LeftClick();

    }
}

void ACoreWeapon::LeftClick(void)
{
   if(OwnerCharacter)
   {
	   if(isLeftClickPressed && !IsOutOfAmmo())
	   {
	   		
                if (IsAiming())
                {
                    if (AimFireAnimationMontage)
                    {
                        // Play Aim Animation and Aim Fire
                        OwnerCharacter->GetArmsHolderSkeletalMesh()->GetAnimInstance()->Montage_Play(AimFireAnimationMontage);
                    }
                }
                else
                {
                    if (NormalFireAnimationMontage)
                    {
	                    // Play Normal Fire Animation and Normal Fire
	                    OwnerCharacter->GetArmsHolderSkeletalMesh()->GetAnimInstance()->Montage_Play(NormalFireAnimationMontage);
                    }
                }

			RemoveAmmo();
			SpawnBullet();
			SpawnCasing();
			Recoil();
	   	
			SetShootingTimerWithDelegate(ShootingTimerHandle, FTimerDelegate::CreateUObject(this, &ACoreWeapon::LeftClick), FireRate, false);

	   }
   }
}


void ACoreWeapon::LeftClick_Released(void)
{
    isLeftClickPressed = false;

    if (OwnerCharacter)
    {
        if (OwnerCharacter->IsWeaponCanFire())
        {

        }
    }
}

void ACoreWeapon::RightClick_Pressed(void)
{
    if(OwnerCharacter){
        OwnerCharacter->SetIsAiming(true);
    }
}

void ACoreWeapon::RightClick(void)
{

}

void ACoreWeapon::RightClick_Released(void)
{
    if(OwnerCharacter){
        OwnerCharacter->SetIsAiming(false);
    }
}

void ACoreWeapon::Reload(void)
{
    currentAmmo = maxCurrentAmmo;
}


void ACoreWeapon::SetShootingTimerWithDelegate(FTimerHandle& TimerHandle, TBaseDelegate<void> ObjectDelegate, float Time, bool bLoop)
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, ObjectDelegate, Time, bLoop);
}


bool ACoreWeapon::IsOutOfAmmo(void)
{
    return currentAmmo < 1;
}

bool ACoreWeapon::IsAiming(void)
{
    if(OwnerCharacter)
    {
        return OwnerCharacter->IsAiming();
    }
    return false;
}


void ACoreWeapon::RemoveAmmo(void)
{
    if(currentAmmo > 0 )
    {
        currentAmmo = currentAmmo - 1;
    }
}

void ACoreWeapon::SpawnBullet(void)
{
	
}

void ACoreWeapon::SpawnCasing(void)
{
	
}

void ACoreWeapon::Recoil(void)
{
	
}