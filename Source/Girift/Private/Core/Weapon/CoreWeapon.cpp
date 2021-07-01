// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapon/CoreWeapon.h"
#include "Core/Character/CoreCharacter.h"
#include "Kismet/GameplayStatics.h"


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

    SpawnPoint_NormalMuzzleFlashPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_MuzzleFlashPosition"));
    SpawnPoint_NormalMuzzleFlashPosition->SetupAttachment(UC_WeaponComponents);


    SpawnPoint_Casing = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_Casing"));
    SpawnPoint_Casing->SetupAttachment(UC_WeaponComponents);

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
	
    bulletsFired = 0;
    bulletsFiredSmoke = 15;

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
    Reload();
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
        if (!(OwnerCharacter->IsWeaponCanFire() && !OwnerCharacter->IsWeaponShooting()) || IsOutOfAmmo())
        {
            if (IsAiming())
            {
                if (AimShoutOutOfAmmoAnimationMontage)
                {
                    // Play Aim Shoot Out Of Ammo Animation 
                    OwnerCharacter->GetArmsHolderSkeletalMesh()->GetAnimInstance()->Montage_Play(AimShoutOutOfAmmoAnimationMontage);

                    if (AimShoutOutOfAmmoSound)
                    {
                        // Play Aim Shoot Out Of Ammo sound  (Aim State)
                        UGameplayStatics::PlaySoundAtLocation(GetWorld(), AimShoutOutOfAmmoSound, GetActorLocation(), GetActorRotation());
                    }
                }
            }
            else
            {
                if (NormalShoutOutOfAmmoAnimationMontage)
                {
                    // Play Normal Shoot Out Of Ammo Animation
                    OwnerCharacter->GetArmsHolderSkeletalMesh()->GetAnimInstance()->Montage_Play(NormalShoutOutOfAmmoAnimationMontage);

                    if (NormalShoutOutOfAmmoSound)
                    {
                        // Play Normal Shoot Out Of Ammo sound (Normal State)
                        UGameplayStatics::PlaySoundAtLocation(GetWorld(), NormalShoutOutOfAmmoSound, GetActorLocation(), GetActorRotation());
                    }
                }
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
   		
       USoundBase* FireSound = GetFireSound();
   	
	   if(isLeftClickPressed && !IsOutOfAmmo())
	   {
	   		
                if (IsAiming())
                {
                    if (AimFireAnimationMontage)
                    {
                        // Play Aim Animation and Aim Fire
                        OwnerCharacter->GetArmsHolderSkeletalMesh()->GetAnimInstance()->Montage_Play(AimFireAnimationMontage);
                        if (FireSound)
                        {
                            // Play shoot sound  (Aim State)
                            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation(), GetActorRotation());
                        }
                    }
                }
                else
                {
                    if (NormalFireAnimationMontage)
                    {
	                    // Play Normal Fire Animation and Normal Fire
	                    OwnerCharacter->GetArmsHolderSkeletalMesh()->GetAnimInstance()->Montage_Play(NormalFireAnimationMontage);
                        if (FireSound)
                        {
                            // Play shoot sound (Normal State)
                            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation(), GetActorRotation());
                        }
                    }
                }

			RemoveAmmo();
			SpawnBullet();
			SpawnCasing();
			Recoil();
	   	
			if(WeaponFireModeStatus == EWeaponFireModeStatus::FullAutomatic)
			{
                SetTimerWithDelegate(ShootingTimerHandle, FTimerDelegate::CreateUObject(this, &ACoreWeapon::LeftClick), FireRate, false);
			}
            else if(WeaponFireModeStatus == EWeaponFireModeStatus::SemiAutomatic)
            {
	            
            }
            else if (WeaponFireModeStatus == EWeaponFireModeStatus::Single)
            {

            }
            else if (WeaponFireModeStatus == EWeaponFireModeStatus::None)
            {

            }

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

    GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
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
	if(OwnerCharacter)
	{
        float AnimationDuration = 0.0f;
        if (IsOutOfAmmo())
        {
            if (ReloadOutOfAmmoAnimationMontage)
            {
            	// Play Reload out of ammo Animation  
                AnimationDuration = OwnerCharacter->GetArmsHolderSkeletalMesh()->GetAnimInstance()->Montage_Play(ReloadOutOfAmmoAnimationMontage);
                OwnerCharacter->Reload_Begin(this);
            }
        }
        else
        {
            if (ReloadAmmoLeftAnimationMontage)
            {
                // Play Reload ammo left Animation  
                AnimationDuration = OwnerCharacter->GetArmsHolderSkeletalMesh()->GetAnimInstance()->Montage_Play(ReloadAmmoLeftAnimationMontage);
                OwnerCharacter->Reload_Begin(this);
            }
        }

        if(AnimationDuration > 0.0f)
        {
            SetTimerWithDelegate(ReloadingTimerHandle, FTimerDelegate::CreateUObject(this, &ACoreWeapon::StopSimulateReloadDelegate) , AnimationDuration, false);
        }

	}
}


void ACoreWeapon::SetTimerWithDelegate(FTimerHandle& TimerHandle, TBaseDelegate<void> ObjectDelegate, float Time, bool bLoop)
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, ObjectDelegate, Time, bLoop);
}

void ACoreWeapon::StopSimulateReloadDelegate(void)
{
	if(OwnerCharacter)
	{
        OwnerCharacter->Reload_End();
        currentAmmo = maxCurrentAmmo;
	}
}


bool ACoreWeapon::IsReloading()
{
    if (OwnerCharacter)
    {
        return OwnerCharacter->IsReloading();
    }
    return false;
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
    if(currentAmmo > 0)
    {
        currentAmmo = currentAmmo - 1;
    }
}

void ACoreWeapon::SpawnBullet(void)
{

	if(OwnerCharacter)
	{
		// Play shoot sound effect
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetFireSound(), GetActorLocation(), GetActorRotation());
        bulletsFired++;

		if(bulletsFired == bulletsFiredSmoke)
		{
           if(PS_MuzzleEmitter)
           {
               UArrowComponent* MuzzleFlashPosition = GetMuzzleFlashPosition();
               if (MuzzleFlashPosition)
               {
                   UGameplayStatics::SpawnEmitterAttached(PS_MuzzleEmitter, MuzzleFlashPosition);
               }
           }
		}

        UArrowComponent* BulletSpawnArrow = GetBulletSpawnComponent();
		// LineTrace
        if(BulletSpawnArrow)
        {
            FHitResult HitResult;
        	
            FVector Start = BulletSpawnArrow->GetComponentLocation();
            FVector End = Start + BulletSpawnArrow->GetForwardVector() * 20000;
        	
            TArray<AActor*> ActorToIgnore;
            ActorToIgnore.Add(this);

        	// line trace if  hit something
            if(UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ActorToIgnore, EDrawDebugTrace::Persistent, HitResult, true))
            {
	            // play bullet impact sound
            	if(BulletImpactSound)
            	{
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletImpactSound, HitResult.ImpactPoint);
            	}
            }
        }

	}
	
}

void ACoreWeapon::SpawnCasing(void)
{
	
}

void ACoreWeapon::Recoil(void)
{
	
}


USoundBase* ACoreWeapon::GetFireSound(void)  
{
    return NormalFireSound;
}

UArrowComponent* ACoreWeapon::GetMuzzleFlashPosition(void)
{
    return SpawnPoint_NormalMuzzleFlashPosition;
}

UArrowComponent* ACoreWeapon::GetBulletSpawnComponent(void) 
{
    return SpawnPoint_Casing;
}