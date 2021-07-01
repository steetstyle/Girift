// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"

#include "Core/Weapon/CoreWeapon.h"
#include "Core/Character/Components/WeaponManagerComponent.h"
#include "Core/Character/Components/CrosshairManagerComponent.h"
#include "Core/Weapon/CoreWeapon.h"
#include "CoreCharacter.generated.h"


UCLASS()
class GIRIFT_API ACoreCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACoreCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetupManagerComponents();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Manager)
    UWeaponManagerComponent*  Component_WeaponManager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Manager)
    UCrosshairManagerComponent*  Component_CrosshairManager;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = SkeletalMesh)
    USkeletalMeshComponent* SM_ArmsHolder;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = SpringArm)
    USpringArmComponent* SpringArm_Main;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = ParentSocket)
    FName PSN_SpringArm_Main;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = CameraComponent)
    UCameraComponent* Camera_SpringArm;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = ParentSocket)
    FName PSN_Camera_SpringArm;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = ArrowComponent)
    UArrowComponent* SpawnPoint_Grenade;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = ArrowComponent)
    UArrowComponent* SpawnPoint_Bullet;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = SceneComponent)
    USceneComponent* UC_FPSViewComponents;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = SpringArm)
    USpringArmComponent* SpringArm_Helmet;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = StaticMesh)
    UStaticMeshComponent* SM_Helmet;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = SpringArm)
    USpringArmComponent* SpringArm_Headset;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = StaticMesh)
    UStaticMeshComponent* SM_Headset;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = StaticMesh)
    UStaticMeshComponent* SM_Googles;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = SceneComponent)
    USceneComponent* UC_KnifeComponents;

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = ParentSocket)
    FName PSN_Knife;

    UPROPERTY(EditAnywhere, Category = StaticMesh)
    UStaticMeshComponent* SM_Knife;

protected:
    //Movement Inputs

    void MoveForward(float axisValue);
    void MoveRight(float axisValue);

    // Mouse Inputs

    void LookUp(float axisValue);
    void LookRight(float axisValue);


    // Interaction Inputs

    void Jump_Pressed();
    void Jump_Released();

    void Sprint_Released();
    void Sprint_Pressed();

protected:
    UPROPERTY(BlueprintReadWrite, Category = Gameplay)
    bool isAiming;

    UPROPERTY(BlueprintReadWrite, Category = Gameplay)
    bool isRunning;

    UPROPERTY(BlueprintReadWrite, Category = Gameplay)
    bool isJumping;

    UPROPERTY(BlueprintReadWrite, Category = Gameplay)
    bool runButtonPressed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    float runSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    float walkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    float footstepDistance_B;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    float footstepDistanceMultiplier_Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    float footstepDistanceMultiplier_Sprinting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    float footstepDistanceMultiplier_SlowWalking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    float footstepDistanceMultiplier_Crounch;

private:
    float footstepDistance;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    float BaseLookUpRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    float BaseTurnRate;

public:
    virtual void Landed(const FHitResult& Hit) override;

protected:
    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isShooting;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isReloading;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isHolstered;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isInspectingWeapon;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isThrowingGrenade;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isMeleeAttacking;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isCloseToWall;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool useLaserSight;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool useFlashLight;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isOutOfAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int currentAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int totalAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int currentGrenades;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    int totalGrenades;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isReloadingOutOfAmmo;

    UPROPERTY(BlueprintReadWrite, Category = WeaponProperties)
    bool isReloadingAmmoLeft;

public:
    virtual USkeletalMeshComponent* GetArmsHolderSkeletalMesh() const;
public:
    //TODO: Weapon Particles need to be added for visual effects

protected:
     // Weapon Actions

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

public:
    virtual void Reload_Begin(class ACoreWeapon* Weapon);
    virtual void Reload_End(void);
    virtual void SetAmmo(void);
    virtual void BulletSpread(void);
public:
    virtual void SetIsAiming(bool status);

public:
    virtual void OnToggleMouseCursor(void);

 public:
     virtual bool IsWeaponCanFire(void);
     virtual bool IsWeaponShooting(void);
     virtual bool IsWeaponOutOfAmmo(void);
     virtual bool IsAiming(void);
     virtual bool IsReloading(void);

};
