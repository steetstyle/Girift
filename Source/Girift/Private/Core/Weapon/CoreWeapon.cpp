// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapon/CoreWeapon.h"
#include "Core/Character/CoreCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACoreWeapon::ACoreWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PSN_Weapon = TEXT("Weapon_Position");
    PSN_Magazine = TEXT("Mag_Position");
    PSN_Slider = TEXT("Slider_Position");
    PSN_Scope = TEXT("Weapon_Position");
    PSN_AimCenter = TEXT("AimMarker");

    UC_Root = CreateDefaultSubobject<USceneComponent>(TEXT("UC_Root"));
    UC_Root->SetupAttachment(GetRootComponent());
    SetRootComponent(UC_Root);

    UC_WeaponComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_WeaponComponents"));
    UC_WeaponComponents->SetupAttachment(UC_Root, PSN_Weapon);

    SM_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Weapon"));
    SM_Weapon->SetupAttachment(UC_WeaponComponents);

    BC_WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BC_WeaponCollider"));

    const FVector BoxExtent_BC_WeaponCollider = FVector(1.0f, 1.0f, 1.0f);
    BC_WeaponCollider->SetBoxExtent(BoxExtent_BC_WeaponCollider,true);

    BC_WeaponCollider->SetupAttachment(UC_WeaponComponents);

    PointLight_MuzzleFlashLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight_MuzzleFlashLight"));
    PointLight_MuzzleFlashLight->SetupAttachment(UC_WeaponComponents);

    SpawnPoint_NormalMuzzleFlashPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_MuzzleFlashPosition"));
    SpawnPoint_NormalMuzzleFlashPosition->SetupAttachment(UC_WeaponComponents);


    SpawnPoint_Casing = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_Casing"));
    SpawnPoint_Casing->SetupAttachment(UC_WeaponComponents);

    UC_MagazineComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_MagazineComponents"));
    UC_MagazineComponents->SetupAttachment(UC_Root, PSN_Magazine);

    SM_Mag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Mag"));
    SM_Mag->SetupAttachment(UC_MagazineComponents);

    SM_Bullets = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Bullets"));
    SM_Bullets->SetupAttachment(UC_MagazineComponents);

    UC_SliderComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_SliderComponents"));
    UC_SliderComponents->SetupAttachment(UC_Root, PSN_Slider);

    SM_Slider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Slider"));
    SM_Slider->SetupAttachment(UC_SliderComponents);

    UC_ScopeComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_ScopeComponents"));
    UC_ScopeComponents->SetupAttachment(UC_Root, PSN_Scope);

    SM_Scope = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Scope"));
    SM_Scope->SetupAttachment(UC_ScopeComponents);

    CC_AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CC_AimCamera"));
    CC_AimCamera->bUsePawnControlRotation = false;
    CC_AimCamera->SetupAttachment(UC_ScopeComponents);

    bHittedSomething = false;

    WeaponFireCasingMode = EWeaponFireCasingMode::None;

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

    MinRecoilIntensity = 10.0f;
    MaxRecoilIntensity = 20.0f;
    MinRecoilLocationX = 0.0f;
    MaxRecoilLocationX = 25.0f;
	
    RecoilMultiplierAim = 0.75f;
    RecoilMultiplierNormal = 1.0f;

    BulletSpreadResetRotation = 3.5f;
    BulletSpreadInterpSpeed = 0.35f;
    BulletSpreadInterpSpeedInRange = 0.05f;
    BulletSpreadIntensity = 1.0f;
	
    MinBulletSpreadLocationX = -5.0f;
    MaxBulletSpreadLocationX = 5.0f;
    MinBulletSpreadLocationY = 5.0f;
    MaxBulletSpreadLocationY = 50.0f;
    MinBulletSpreadLocationZ = -15.0f;
    MaxBulletSpreadLocationZ = 15.0f;

    ArmRotationInterpSpeed = 0.01f;

    MinPitchClamp = -65.0f;
    MaxPitchClamp = 65.0f;
    BulletSpreadResetDuration = 3.5f;
    RecoilResetDuration = 5.0f;

    BulletSpreadAimingMultiplier = 0.08f;
    BulletSpreadNormalMultiplier = 0.15f;

    SmoothFOVInterpSpeed = 0.15f;

    bAutoCenterAimCamera = true;

    bAutoCenterAimCameraKeepX = false;
    bAutoCenterAimCameraKeepY = false;
    bAutoCenterAimCameraKeepZ = false;


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

    SmoothFOVIncrease();
    ResetArmRotation();
    ClampArmRotation();
	BulletSpread();

}

void ACoreWeapon::InitializeWeaponForCharacter(ACoreCharacter* Character)
{
    UE_LOG(LogTemp, Warning, TEXT("InitializeWeaponForCharacter Called"));

    OwnerCharacter = Character;
    if(OwnerCharacter){
        UC_WeaponComponents->AttachToComponent(OwnerCharacter->GetArmsHolderSkeletalMesh(), FAttachmentTransformRules::KeepRelativeTransform, PSN_Weapon);
        UC_MagazineComponents->AttachToComponent(OwnerCharacter->GetArmsHolderSkeletalMesh(), FAttachmentTransformRules::KeepRelativeTransform, PSN_Magazine);
        UC_SliderComponents->AttachToComponent(OwnerCharacter->GetArmsHolderSkeletalMesh(), FAttachmentTransformRules::KeepRelativeTransform, PSN_Slider);
        UC_ScopeComponents->AttachToComponent(OwnerCharacter->GetArmsHolderSkeletalMesh(), FAttachmentTransformRules::KeepRelativeTransform, PSN_Scope);
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
        UC_ScopeComponents->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
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
       if (IsOutOfAmmo())
       {
           OwnerCharacter->Shoot_End();
           return;
       }
   	
       OwnerCharacter->Shoot_Begin(this);

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
                SetTimerWithDelegate(ShootingTimerHandle, FTimerDelegate::CreateUObject(this, &ACoreWeapon::LeftClick_Released), FireRate, false);
            }
            else if (WeaponFireModeStatus == EWeaponFireModeStatus::Single)
            {
                SetTimerWithDelegate(ShootingTimerHandle, FTimerDelegate::CreateUObject(this, &ACoreWeapon::LeftClick_Released), FireRate, false);
            }
            else if (WeaponFireModeStatus == EWeaponFireModeStatus::None)
            {
                SetTimerWithDelegate(ShootingTimerHandle, FTimerDelegate::CreateUObject(this, &ACoreWeapon::LeftClick_Released), FireRate, false);
            }

            bHittedSomething = false;

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
        OwnerCharacter->Shoot_End();
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
            if (BulletSpawnArrow)
            {

                const FVector CharacterCameraLocationStart = OwnerCharacter->SpawnPoint_Bullet->GetComponentLocation();
                const FVector CharacterCameraLocationEnd = CharacterCameraLocationStart + OwnerCharacter->SpawnPoint_Bullet->GetForwardVector() * 20000;


                FHitResult HitResult;

                TArray<AActor*> ActorToIgnore;
                ActorToIgnore.Add(this);

                // line trace if  hit something
                if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), CharacterCameraLocationStart, CharacterCameraLocationEnd, ETraceTypeQuery::TraceTypeQuery1, false, ActorToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
                {
                    const FVector Start = SpawnPoint_Casing->GetComponentLocation();
                    const FVector End = HitResult.ImpactPoint;
                    ProjectileLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, HitResult.ImpactPoint);


                    // play bullet impact sound if not  projectiled mode
                    if (BulletImpactSound &&  WeaponFireCasingMode == EWeaponFireCasingMode::None)
                    {
                        UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletImpactSound, HitResult.ImpactPoint);
                    }


                    if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ActorToIgnore, EDrawDebugTrace::Persistent, HitResult, true))
                    {

                    }

                }
            }

	}
	
}

void ACoreWeapon::SpawnCasing(void)
{
    if (WeaponFireCasingMode != EWeaponFireCasingMode::None)
    {
        FVector WeaponLocation = SpawnPoint_Casing->GetComponentLocation();
    	
        AActor* InstantlySpawnedProjectile;

    	if(WeaponFireCasingMode == EWeaponFireCasingMode::Big && WeaponFireBigCasingClass){

            InstantlySpawnedProjectile =  GetWorld()->SpawnActor(WeaponFireBigCasingClass, &WeaponLocation, &ProjectileLookAtRotation);
    	}
        else if(WeaponFireCasingMode == EWeaponFireCasingMode::Small && WeaponFireSmallCasingClass){
        	
            InstantlySpawnedProjectile =  GetWorld()->SpawnActor(WeaponFireSmallCasingClass, &WeaponLocation, &ProjectileLookAtRotation);

        }
    }
}

void ACoreWeapon::Recoil(void)
{
	if(OwnerCharacter)
	{

        const FVector SpringArmLocation = OwnerCharacter->SpringArm_Main->GetRelativeLocation();

        const float ClampedX = UKismetMathLibrary::Clamp(SpringArmLocation.X, MinRecoilLocationX, MaxRecoilLocationX);

        const float RandomIntensity = UKismetMathLibrary::RandomFloatInRange(MinRecoilIntensity, MaxRecoilIntensity);
		
        if (IsAiming())
        {
            const float InterpolatedXLocation = UKismetMathLibrary::FInterpTo(ClampedX, RandomIntensity * RecoilMultiplierAim,1.0f, 0.05f);
            const FVector NewRelativeLocation = FVector(InterpolatedXLocation, SpringArmLocation.Y, SpringArmLocation.Z);
            OwnerCharacter->SpringArm_Main->SetRelativeLocation(NewRelativeLocation);
        }
        else

        {
            const float NewClampedIntesity = ClampedX + RandomIntensity;
            const float InterpolatedXLocation = UKismetMathLibrary::FInterpTo(NewClampedIntesity, RandomIntensity * RecoilMultiplierNormal, 1.0f, 0.05f);
            const FVector NewRelativeLocation = FVector(InterpolatedXLocation, SpringArmLocation.Y, SpringArmLocation.Z);
            OwnerCharacter->SpringArm_Main->SetRelativeLocation(NewRelativeLocation);
        }
	}
}

void ACoreWeapon::BulletSpread()
{
	if(OwnerCharacter && OwnerCharacter->IsValidLowLevel())
	{

        if (OwnerCharacter->IsWeaponShooting() && !IsOutOfAmmo())
        {

            USkeletalMeshComponent* CharacterArmsHolder = OwnerCharacter->GetArmsHolderSkeletalMesh();
            if(CharacterArmsHolder)
            {

                const float ArmsHolderRotationPitch = CharacterArmsHolder->GetRelativeRotation().Pitch;
                const float ArmsHolderInRange = UKismetMathLibrary::InRange_FloatFloat(ArmsHolderRotationPitch, -65, 65);
            	
                if (ArmsHolderInRange)
                {

                    if(IsAiming())
                    {
                        BulletSpreadIntensity = BulletSpreadAimingMultiplier;
                    }
                    else
                    {
                        BulletSpreadIntensity = BulletSpreadNormalMultiplier;
                    }

                    const float RandomBulletSpreadX = UKismetMathLibrary::RandomFloatInRange(MinBulletSpreadLocationX, MaxBulletSpreadLocationX);
                    const float RandomBulletSpreadY = UKismetMathLibrary::RandomFloatInRange(MinBulletSpreadLocationY, MaxBulletSpreadLocationY);
                    const float RandomBulletSpreadZ = UKismetMathLibrary::RandomFloatInRange(MinBulletSpreadLocationZ, MaxBulletSpreadLocationZ);

                    const float InterpedBulletSpreadX = UKismetMathLibrary::FInterpTo(0.0f, RandomBulletSpreadX, 1.0f, BulletSpreadInterpSpeedInRange);
                    const float InterpedBulletSpreadY = UKismetMathLibrary::FInterpTo(0.0f, RandomBulletSpreadY, 1.0f, BulletSpreadInterpSpeedInRange);
                    const float InterpedBulletSpreadZ = UKismetMathLibrary::FInterpTo(0.0f, RandomBulletSpreadZ, 1.0f, BulletSpreadInterpSpeedInRange);

                    const float MultipliedBulletSpreadX = InterpedBulletSpreadX * BulletSpreadIntensity;
                    const float MultipliedBulletSpreadY = InterpedBulletSpreadY * BulletSpreadIntensity;
                    const float MultipliedBulletSpreadZ = InterpedBulletSpreadZ * BulletSpreadIntensity;

                    const FRotator BulletSpreadRotation = FRotator(
                        MultipliedBulletSpreadY,
                        MultipliedBulletSpreadZ,
                        MultipliedBulletSpreadX
                    );

                    CharacterArmsHolder->AddRelativeRotation(BulletSpreadRotation);
                }
                else
                {
                    const FRotator ArmsHolderWorldRotation = CharacterArmsHolder->GetComponentRotation();

                    const float NewRoll = UKismetMathLibrary::FInterpTo(ArmsHolderWorldRotation.Roll, 0.0f, BulletSpreadResetDuration, BulletSpreadInterpSpeed);
                    const float NewPitch = UKismetMathLibrary::FInterpTo(ArmsHolderWorldRotation.Pitch, 0.0f, BulletSpreadResetDuration, BulletSpreadInterpSpeed);
                    const float NewYaw = UKismetMathLibrary::FInterpTo(ArmsHolderWorldRotation.Yaw, 0.0f, BulletSpreadResetDuration, BulletSpreadInterpSpeed);

                    const FRotator NewRotation = FRotator(NewPitch, NewYaw, NewRoll);

                    CharacterArmsHolder->SetRelativeRotation(NewRotation);
                }
            }
		}
	}
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

void ACoreWeapon::ClampArmRotation(void)
{
    if (OwnerCharacter && OwnerCharacter->IsValidLowLevel())
    {
        USkeletalMeshComponent* OwnerCharacterMesh = OwnerCharacter->GetMainSkeletalMesh();

        if (OwnerCharacterMesh)
        {
            const FRotator OwnerCharacterMeshRotation = OwnerCharacterMesh->GetRelativeRotation();
            const float NewPitch = UKismetMathLibrary::ClampAngle(OwnerCharacterMeshRotation.Pitch, MinPitchClamp, MaxPitchClamp);

            const FRotator NewRotation = FRotator(NewPitch, 0.0f, 0.0f);

            OwnerCharacterMesh->SetRelativeRotation(NewRotation);
        }

    }
}

void ACoreWeapon::ResetArmRotation(void)
{
    if(OwnerCharacter && OwnerCharacter->IsValidLowLevel())
    {
        if(!(OwnerCharacter->IsWeaponShooting() && !IsOutOfAmmo()))
        {
            USkeletalMeshComponent* OwnerCharacterArmsHolder = OwnerCharacter->GetArmsHolderSkeletalMesh();

        	if(OwnerCharacterArmsHolder)
        	{
                const FRotator OwnerCharacterArmsHolderRotation = OwnerCharacterArmsHolder->GetRelativeRotation();

                const float NewRotationPitch = UKismetMathLibrary::FInterpTo(OwnerCharacterArmsHolderRotation.Pitch, 0.0f, BulletSpreadResetDuration, ArmRotationInterpSpeed);
                const float NewRotationYaw = UKismetMathLibrary::FInterpTo(OwnerCharacterArmsHolderRotation.Yaw, 0.0f, BulletSpreadResetDuration, ArmRotationInterpSpeed);
                const float NewRotationRoll = UKismetMathLibrary::FInterpTo(OwnerCharacterArmsHolderRotation.Roll, 0.0f, BulletSpreadResetDuration, ArmRotationInterpSpeed);

                const FRotator NewRotation = FRotator(NewRotationPitch, NewRotationYaw,NewRotationRoll);

                OwnerCharacterArmsHolder->SetRelativeRotation(NewRotation);
        	}

            // Reset Spring Arm Location(Used for recoil)

            USpringArmComponent* OwnerCharacterSpringArm = OwnerCharacter->GetMainSpringArm();

        	if(OwnerCharacterSpringArm)
        	{
                const FVector OwnerCharacterSpringArmLocation = OwnerCharacterSpringArm->GetRelativeLocation();

                const float NewLocationX = UKismetMathLibrary::FInterpTo(OwnerCharacterSpringArmLocation.X, 0.0f, RecoilResetDuration, ArmRotationInterpSpeed);
                const FVector NewRelativeLocation = FVector(NewLocationX,0.0f,0.0f);

                OwnerCharacterSpringArm->SetRelativeLocation(NewRelativeLocation);
        	}
        }
    }
}

void ACoreWeapon::SmoothFOVIncrease(void)
{
	if(OwnerCharacter && OwnerCharacter->IsValidLowLevel())
	{
        UCameraComponent* OwnerCharacterCameraComponent = OwnerCharacter->GetMainCamera();
        const FVector OwnerCharacterCameraPosition = OwnerCharacterCameraComponent->GetRelativeLocation();
		
        if(IsAiming() && !OwnerCharacter->IsWeaponInspecting() && !OwnerCharacter->IsRunning() && !IsReloading() && !IsOutOfAmmo())
		{

            // if is aiming go aim camera position
        	
            const FVector AimCameraPosition = CC_AimCamera->GetRelativeLocation();

            const float NewLocationX = UKismetMathLibrary::FInterpTo(OwnerCharacterCameraPosition.X, AimCameraPosition.X, 1.0f, SmoothFOVInterpSpeed);
            const float NewLocationY = UKismetMathLibrary::FInterpTo(OwnerCharacterCameraPosition.Y, AimCameraPosition.Y, 1.0f, SmoothFOVInterpSpeed);
            const float NewLocationZ = UKismetMathLibrary::FInterpTo(OwnerCharacterCameraPosition.Z, AimCameraPosition.Z, 1.0f, SmoothFOVInterpSpeed);

            const FVector NewLocation = FVector(NewLocationX, NewLocationY, NewLocationZ);
            OwnerCharacterCameraComponent->SetRelativeLocation(NewLocation);
         
            const float NewFieldOfView = UKismetMathLibrary::FInterpTo(OwnerCharacterCameraComponent->FieldOfView, CC_AimCamera->FieldOfView, 1.0f, SmoothFOVInterpSpeed);

            OwnerCharacterCameraComponent->SetFieldOfView(NewFieldOfView);

		}
        else
        {
	        // if is not aiming turn back owner character position
            const float NewLocationX = UKismetMathLibrary::FInterpTo(OwnerCharacterCameraPosition.X,0.0f, 1.0f, SmoothFOVInterpSpeed);
            const float NewLocationY = UKismetMathLibrary::FInterpTo(OwnerCharacterCameraPosition.Y,0.0f, 1.0f, SmoothFOVInterpSpeed);
            const float NewLocationZ = UKismetMathLibrary::FInterpTo(OwnerCharacterCameraPosition.Z,0.0f, 1.0f, SmoothFOVInterpSpeed);

            const FVector NewLocation = FVector(NewLocationX, NewLocationY, NewLocationZ);
            OwnerCharacterCameraComponent->SetRelativeLocation(NewLocation);

            const float NewFieldOfView = UKismetMathLibrary::FInterpTo(OwnerCharacterCameraComponent->FieldOfView, OwnerCharacter->DefaultFieldOfView, 1.0f, SmoothFOVInterpSpeed);

            OwnerCharacterCameraComponent->SetFieldOfView(NewFieldOfView);
        }
	}
}