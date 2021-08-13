// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundBase.h"
#include "Components/PointLightComponent.h"
#include "CoreWeapon.generated.h"

UENUM()
enum class EWeaponFireModeStatus : uint8
{
    None     UMETA(DisplayName = "None"),
    Single    UMETA(DisplayName = "Single"),
    SemiAutomatic      UMETA(DisplayName = "Semi Automatic"),
    FullAutomatic   UMETA(DisplayName = "Full Automatic"),
};

UENUM()
enum class EWeaponFireCasingMode : uint8
{
    None     UMETA(DisplayName = "None"),
    Big    UMETA(DisplayName = "Big"),
    Small      UMETA(DisplayName = "Small"),
};

UCLASS()
class ACoreWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoreWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void InitializeWeaponForCharacter(class ACoreCharacter* Character);
    virtual void DeinitializeWeapon();

protected:
    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isOutOfAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int currentAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int maxCurrentAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int totalAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isReloadingOutOfAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isReloadingAmmoLeft;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int bulletsFired;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int bulletsFiredSmoke;
	
    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool bProjectileMode;


protected:
    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isLeftClickPressed;
	
    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isRightClickPressed;

private:
    USceneComponent* UC_Root;


public:
    UPROPERTY(EditDefaultsOnly, Category = RecoilProperties)
        float MinRecoilIntensity;
	
    UPROPERTY(EditDefaultsOnly, Category = RecoilProperties)
        float MaxRecoilIntensity;
	
    UPROPERTY(EditDefaultsOnly, Category = RecoilProperties)
        float MinRecoilLocationX;
	
    UPROPERTY(EditDefaultsOnly, Category = RecoilProperties)
        float MaxRecoilLocationX;

    UPROPERTY(EditDefaultsOnly, Category = RecoilProperties)
        float RecoilMultiplierAim;
	
    UPROPERTY(EditDefaultsOnly, Category = RecoilProperties)
        float RecoilMultiplierNormal;


public:
    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float BulletSpreadResetRotation;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float BulletSpreadInterpSpeed;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float BulletSpreadInterpSpeedInRange;
    
    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float BulletSpreadIntensity;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float BulletSpreadAimingMultiplier;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float BulletSpreadNormalMultiplier;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float MinBulletSpreadLocationX;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float MaxBulletSpreadLocationX;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float MinBulletSpreadLocationY;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float MaxBulletSpreadLocationY;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float MinBulletSpreadLocationZ;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float MaxBulletSpreadLocationZ;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float BulletSpreadResetDuration;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float MinPitchClamp;

    UPROPERTY(EditDefaultsOnly, Category = BulletSpreadProperties)
        float MaxPitchClamp;

    UPROPERTY(EditDefaultsOnly, Category = RecoilProperties)
        float ArmRotationInterpSpeed;

    UPROPERTY(EditDefaultsOnly, Category = RecoilProperties)
        float RecoilResetDuration;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
    EWeaponFireModeStatus WeaponFireModeStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
    EWeaponFireCasingMode WeaponFireCasingMode;

    UPROPERTY(EditAnywhere, Category = SceneComponent)
    TSubclassOf<AActor> WeaponFireSmallCasingClass;

    UPROPERTY(EditAnywhere, Category = SceneComponent)
    TSubclassOf<AActor> WeaponFireBigCasingClass;
	
    UPROPERTY(EditAnywhere, Category = SceneComponent)
    USceneComponent* UC_WeaponComponents;

    UPROPERTY(EditAnywhere, Category = ParentSocket)
    FName PSN_Weapon;

    UPROPERTY(EditAnywhere, Category = StaticMesh)
    UStaticMeshComponent* SM_Weapon;

    UPROPERTY(EditAnywhere, Category = Collider)
    UBoxComponent* BC_WeaponCollider;

    UPROPERTY(EditAnywhere, Category = PointLigth)
    UPointLightComponent* PointLight_MuzzleFlashLight;

    UPROPERTY(EditAnywhere, Category = ArrowComponent)
    UArrowComponent* SpawnPoint_NormalMuzzleFlashPosition;

    UPROPERTY(EditAnywhere, Category = SceneComponent)
    USceneComponent* UC_MagazineComponents;

    UPROPERTY(EditAnywhere, Category = ParentSocket)
    FName PSN_Magazine;

    UPROPERTY(EditAnywhere, Category = StaticMesh)
    UStaticMeshComponent* SM_Mag;

    UPROPERTY(EditAnywhere, Category = StaticMesh)
    UStaticMeshComponent* SM_Bullets;

    UPROPERTY(EditAnywhere, Category = SceneComponent)
    USceneComponent* UC_SliderComponents;

    UPROPERTY(EditAnywhere, Category = ParentSocket)
    FName PSN_Slider;

    UPROPERTY(EditAnywhere, Category = StaticMesh)
    UStaticMeshComponent* SM_Slider;

    UPROPERTY(EditAnywhere, Category = ArrowComponent)
    UArrowComponent* SpawnPoint_Casing;

    UPROPERTY(EditAnywhere, Category = SceneComponent)
    USceneComponent* UC_ScopeComponents;
	
    UPROPERTY(EditAnywhere, Category = ParentSocket)
    FName PSN_Scope;

    UPROPERTY(EditAnywhere, Category = StaticMesh)
    UStaticMeshComponent* SM_Scope;


public:
    UPROPERTY(EditAnywhere, Category = WeaponSound)
    USoundBase* AimShoutOutOfAmmoSound;

    UPROPERTY(EditAnywhere, Category = WeaponSound)
    USoundBase* NormalShoutOutOfAmmoSound;

    UPROPERTY(EditAnywhere, Category = WeaponSound)
    USoundBase* NormalFireSound;

    UPROPERTY(EditAnywhere, Category = WeaponSound)
    USoundBase* SilencerFireSound;

    UPROPERTY(EditAnywhere, Category = WeaponSound)
    USoundBase* BulletImpactSound;

public:
    UPROPERTY(EditAnywhere, Category = WeaponParticle)
    UParticleSystem* PS_MuzzleEmitter;


public:
    UPROPERTY(EditAnywhere, Category = WeaponAnimation)
    TSubclassOf<UAnimInstance> WeaponAnimClass;

public:
    UPROPERTY(EditAnywhere, Category = WeaponAnimation)
    UAnimMontage* AimFireAnimationMontage;
	
    UPROPERTY(EditAnywhere, Category = WeaponAnimation)
    UAnimMontage* NormalFireAnimationMontage;

    UPROPERTY(EditAnywhere, Category = WeaponAnimation)
    UAnimMontage* AimShoutOutOfAmmoAnimationMontage;

    UPROPERTY(EditAnywhere, Category = WeaponAnimation)
    UAnimMontage* NormalShoutOutOfAmmoAnimationMontage;

    UPROPERTY(EditAnywhere, Category = WeaponAnimation)
    UAnimMontage* ReloadOutOfAmmoAnimationMontage;

    UPROPERTY(EditAnywhere, Category = WeaponAnimation)
    UAnimMontage* ReloadAmmoLeftAnimationMontage;

public:
    UPROPERTY(EditAnywhere, Category = Aim)
    UCameraComponent* CC_AimCamera;

    UPROPERTY(EditAnywhere, Category = Aim)
    float AimFieldOfView;
	
    UPROPERTY(EditAnywhere, Category = Aim)
    float SmoothFOVInterpSpeed;
	
    UPROPERTY(EditAnywhere, Category = Aim)
	bool bAutoCenterAimCamera;

    UPROPERTY(EditAnywhere, Category = Aim)
    FName PSN_AimCenter;

    UPROPERTY(EditAnywhere, Category = Aim)
    bool bAutoCenterAimCameraKeepX;

    UPROPERTY(EditAnywhere, Category = Aim)
    bool bAutoCenterAimCameraKeepY;

    UPROPERTY(EditAnywhere, Category = Aim)
    bool bAutoCenterAimCameraKeepZ;
	

public:
    UPROPERTY(EditAnywhere, Category = WeaponSettings)
    float FireRate;

private:
    class ACoreCharacter* OwnerCharacter;

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
    virtual void LeftClick(void);
    virtual void LeftClick_Released(void);

    virtual void RightClick_Pressed(void);
    virtual void RightClick(void);
    virtual void RightClick_Released(void);

public:
    virtual void Reload(void);

protected:
    FTimerHandle ShootingTimerHandle;
    FTimerHandle ReloadingTimerHandle;
    virtual void StopSimulateReloadDelegate(void);

    virtual void SetTimerWithDelegate(FTimerHandle& TimerHandle, TBaseDelegate<void> ObjectDelegate, float Time, bool bLoop);

private:
    FRotator ProjectileLookAtRotation;
    bool bHittedSomething;

public:
    virtual bool IsOutOfAmmo(void);
    virtual bool IsAiming(void);
    virtual bool IsReloading(void);

public:
    virtual USoundBase* GetFireSound(void);
    virtual UArrowComponent* GetMuzzleFlashPosition(void);
	virtual UArrowComponent* GetBulletSpawnComponent(void);

protected:
    virtual void RemoveAmmo(void);
    virtual void SpawnBullet(void);
    virtual void SpawnCasing(void);
    virtual void Recoil(void);
    virtual void BulletSpread(void);
	
    // Clamp fps view rotation angle
    virtual void ClampArmRotation(void);
	
	// Reset Arm Mesh Rotation (Used for bullet spread)
    virtual void ResetArmRotation(void);

    //Smooth FOV Increase For Aim
    virtual void SmoothFOVIncrease(void);

};
