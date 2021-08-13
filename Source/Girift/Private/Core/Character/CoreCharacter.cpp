// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/CoreCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/Character/Components/WeaponManagerComponent.h"
#include "Core/Character/CorePlayerController.h"


// Sets default values
ACoreCharacter::ACoreCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // FPS Constructor
    // FPS Constructor
    // FPS Constructor
    // FPS Constructor
    // FPS Constructor

	// Parent Socket Names
    PSN_SpringArm_Main = TEXT("FPS_Camera_Position");
    PSN_Camera_SpringArm = TEXT("SpringEndpoint");
    PSN_Knife = TEXT("Knife_Position");

	SM_ArmsHolder = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsHolder"));
    SM_ArmsHolder->SetupAttachment(GetMesh());
    SM_ArmsHolder->SetRelativeLocation(FVector(30.0f,0.0f, 50.0f));

    SpringArm_Main = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Main"));
    SpringArm_Main->TargetArmLength = 0.0f;
    SpringArm_Main->bInheritRoll = false;
    SpringArm_Main->bEnableCameraLag = true;
    SpringArm_Main->bEnableCameraRotationLag = true;
    SpringArm_Main->CameraLagSpeed = 25.0f;
    SpringArm_Main->CameraRotationLagSpeed = 25.0f;
    SpringArm_Main->CameraLagMaxDistance = 1.5f;
    SpringArm_Main->SetupAttachment(SM_ArmsHolder, PSN_SpringArm_Main);

    SM_ArmsHolder->bOnlyOwnerSee = true;


    /* SPRING ARM Attahment START*/

        Camera_SpringArm = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera_SpringArm"));
        //Camera_SpringArm->SetScale(FVector(0.5f, 0.5f, 0.5f));
        Camera_SpringArm->SetFieldOfView(85.f);
        Camera_SpringArm->SetupAttachment(SpringArm_Main, PSN_Camera_SpringArm);

        SpawnPoint_Grenade = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_Grenade"));
        SpawnPoint_Grenade->SetupAttachment(Camera_SpringArm);

        SpawnPoint_Bullet = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint_Bullet"));
        SpawnPoint_Bullet->SetupAttachment(Camera_SpringArm);

        UC_FPSViewComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_FPSViewComponents"));
        UC_FPSViewComponents->SetupAttachment(Camera_SpringArm);

            /* UC_FPSViewComponents Attachments START*/

            SpringArm_Helmet = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Helmet"));
            SpringArm_Helmet->SetupAttachment(UC_FPSViewComponents);

            SM_Helmet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Helmet"));
            SM_Helmet->SetupAttachment(SpringArm_Helmet);

            SpringArm_Headset = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_Headset"));
            SpringArm_Headset->SetupAttachment(UC_FPSViewComponents);

            SM_Headset = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Headset"));
            SM_Headset->SetupAttachment(SpringArm_Headset);

            SM_Googles = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Googles"));
            SM_Googles->SetupAttachment(UC_FPSViewComponents);

            /* UC_FPSViewComponents Attachments END*/

        UC_KnifeComponents = CreateDefaultSubobject<USceneComponent>(TEXT("UC_KnifeComponents"));
        UC_KnifeComponents->SetupAttachment(SM_ArmsHolder, PSN_Knife);

            /* UC_KnifeComponents Attachments START */

                SM_Knife = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Knife"));
                SM_Knife->SetupAttachment(UC_KnifeComponents);
            /* UC_KnifeComponents Attachments END */

    /* SPRING ARM Attahment END */

    // TPS Constructor
    // TPS Constructor
    // TPS Constructor
    // TPS Constructor
    // TPS Constructor
    // TPS Constructor

    SM_ThirdPerson = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_ThirdPerson"));
    SM_ThirdPerson->SetupAttachment(GetRootComponent());
    SM_ThirdPerson->SetRelativeLocation(FVector(0.f,0.f,-88.0f));

    SM_ThirdPerson->bOwnerNoSee = true;


    BaseTurnRate = 10.0f;
    BaseLookUpRate = 10.0f;

    isAiming = false;
    isRunning = false;
    isJumping = false;

    isReloading = false;
    isAiming  = false;
    isInspectingWeapon  = false;
    isThrowingGrenade  = false;
    isMeleeAttacking  = false;
    isCloseToWall = false;
    isOutOfAmmo = false;
    isReloadingOutOfAmmo = false;
    isReloadingAmmoLeft = false;

    totalAmmo = 0;
    currentAmmo = 0;

    totalGrenades = 0;
    currentGrenades = 0;

    runButtonPressed = false;

    runSpeed = 3000.0f;
    walkSpeed = 1500.0f;

    GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

    footstepDistance_B = 350.f;
    footstepDistance = footstepDistance_B;
    footstepDistanceMultiplier_Normal = 10.0f;
    footstepDistanceMultiplier_Sprinting = 10.0f;
    footstepDistanceMultiplier_SlowWalking = 10.0f;
    footstepDistanceMultiplier_Crounch = 10.0f;

    DefaultFieldOfView = 95.f;

    ACoreCharacter::SetupManagerComponents();
}

// Called when the game starts or when spawned
void ACoreCharacter::BeginPlay()
{
	Super::BeginPlay();


    if (!(IsLocallyControlled()))
    {
        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = true;
        bUseControllerRotationRoll = true;
    }
    else
    {
        bUseControllerRotationPitch = true;
        bUseControllerRotationYaw = true;
        bUseControllerRotationRoll = true;

   }
}

// Called every frame
void ACoreCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACoreCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Vertical", this, &ACoreCharacter::MoveForward);
    PlayerInputComponent->BindAxis("Horizontal", this, &ACoreCharacter::MoveRight);

    PlayerInputComponent->BindAxis("MouseLookVertical", this, &ACoreCharacter::LookUp);
    PlayerInputComponent->BindAxis("MouseLookHorizontal", this, &ACoreCharacter::LookRight);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACoreCharacter::Sprint_Pressed);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACoreCharacter::Sprint_Released);

    PlayerInputComponent->BindAction("Jump", IE_Pressed ,this, &ACoreCharacter::Jump_Pressed);
    PlayerInputComponent->BindAction("Jump", IE_Released ,this, &ACoreCharacter::Jump_Released);

   if(Component_WeaponManager)
   {
       Component_WeaponManager->SetupPlayerInputComponent(PlayerInputComponent);
   }
}

void ACoreCharacter::SetupManagerComponents(void) {
    if(Component_WeaponManager == nullptr) {
        Component_WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>("WeaponManagerComponent");
    }
    if(Component_CrosshairManager == nullptr) {
        Component_CrosshairManager = CreateDefaultSubobject<UCrosshairManagerComponent>("CrosshairManagerComponent");
    }
}

void ACoreCharacter::MoveForward(float axisValue){
    if (axisValue != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorForwardVector(), axisValue);
    }
}

void ACoreCharacter::MoveRight(float axisValue){
    if (axisValue != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorRightVector(), axisValue);
    }
}

void ACoreCharacter::LookUp(float axisValue){
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(axisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACoreCharacter::LookRight(float axisValue){
    // calculate delta for this frame from the rate information
    AddControllerYawInput(axisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACoreCharacter::Sprint_Pressed(){
    if(!isAiming){
        runButtonPressed = true;
        GetCharacterMovement()->MaxWalkSpeed = runSpeed;
        isRunning = true;
        footstepDistance = footstepDistance_B * footstepDistanceMultiplier_Sprinting;
    }
}

void ACoreCharacter::Sprint_Released(){
    runButtonPressed = false;
    GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
    isRunning = false;
    footstepDistance = footstepDistance_B * footstepDistanceMultiplier_Normal;
}

void ACoreCharacter::Jump_Pressed(){
    Jump();
    isJumping = true;
    runButtonPressed = false;
}


void ACoreCharacter::Jump_Released(){

}


void ACoreCharacter::Landed(const FHitResult& Hit){
    Super::Landed(Hit);

    if(isRunning) runButtonPressed = true;
    isJumping = false;
}

void ACoreCharacter::Reload_Pressed(void){
    if(!(
            isShooting ||
            isReloading ||
            isAiming ||
            isHolstered ||
            isInspectingWeapon ||
            isThrowingGrenade ||
            isMeleeAttacking ||
            isCloseToWall)){
            }
}

void ACoreCharacter::Reload_Released(void){

}

void ACoreCharacter::InspectWeapon_Pressed(void){

}

void ACoreCharacter::InspectWeapon_Released(void){

}

void ACoreCharacter::HolsterWeapon_Pressed(void){

}

void ACoreCharacter::HolsterWeapon_Released(void){

}

void ACoreCharacter::ToggleLaserSight_Pressed(void){

}

void ACoreCharacter::ToggleLaserSight_Released(void){

}

void ACoreCharacter::ToggleFlashlight_Pressed(void){

}

void ACoreCharacter::ToggleFlashlight_Released(void){

}

void ACoreCharacter::Reload_Begin(ACoreWeapon* Weapon){
    if(Weapon)
    {
        if (Weapon->IsOutOfAmmo()) {
            if (!Weapon->IsReloading()) {
                isReloadingAmmoLeft = false;
                isReloadingOutOfAmmo = true;
                isReloading = true;
                //currentAmmo = totalAmmo;

                //TODO: Play Sound Out Of Ammo
            }
        }
        else {
            if (!Weapon->IsReloading()) {
                isReloadingAmmoLeft = true;
                isReloadingOutOfAmmo = false;
                isReloading = true;
                //currentAmmo = totalAmmo;

                //TODO: Play Sound Ammo Left
            }
        }
    }
}

void ACoreCharacter::Reload_End(void)
{
    isReloadingAmmoLeft = false;
    isReloadingOutOfAmmo = false;
    isReloading = false;
}

void ACoreCharacter::SetIsAiming(bool status){
    isAiming = status;
}

void ACoreCharacter::SetAmmo(void){
    currentGrenades = totalGrenades;
    currentAmmo = totalAmmo;
}

void ACoreCharacter::BulletSpread(void){
    if(isShooting && !isOutOfAmmo){
       //TODO: check Arm Rotation is within clamp range
    }
}

void ACoreCharacter::Shoot_Begin(ACoreWeapon* Weapon)
{
    if (Weapon)
    {
    	if(!Weapon->IsOutOfAmmo())
    	{
            isShooting = true;

    	}
    }
}

void ACoreCharacter::Shoot_End(void)
{
    isShooting = false;
}



void ACoreCharacter::OnToggleMouseCursor(void)
{
    ACorePlayerController* PC = Cast<ACorePlayerController>(GetController());
    if (PC->bShowMouseCursor)
    {
        PC->bShowMouseCursor = false;
        FInputModeGameOnly GameOnlyInputMode;
        GameOnlyInputMode.SetConsumeCaptureMouseDown(true);
        PC->SetInputMode(GameOnlyInputMode);
    }
    else
    {
        PC->bShowMouseCursor = true;
        FInputModeGameAndUI GameAndUIInputMode;
        GameAndUIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
        PC->SetInputMode(GameAndUIInputMode);
    }
}

USpringArmComponent* ACoreCharacter::GetMainSpringArm() const {
    return SpringArm_Main;
}

USkeletalMeshComponent* ACoreCharacter::GetMainSkeletalMesh() const {
    return GetMesh();
}

USkeletalMeshComponent* ACoreCharacter::GetArmsHolderSkeletalMesh()  const {
    return SM_ArmsHolder;
}

UCameraComponent* ACoreCharacter::GetMainCamera() const
{
    return Camera_SpringArm;
}

bool ACoreCharacter::IsWeaponCanFire()
{
    if (isReloading || IsWeaponOutOfAmmo() || isHolstered || isThrowingGrenade || isRunning || isCloseToWall || isInspectingWeapon || isMeleeAttacking)
    {
        return false;
    }
    return true;
}

bool ACoreCharacter::IsWeaponShooting(void)
{
    return isShooting;
}

bool ACoreCharacter::IsWeaponOutOfAmmo(void)
{
    return isOutOfAmmo;
}

bool ACoreCharacter::IsAiming(void)
{
    return isAiming;
}

bool ACoreCharacter::IsReloading(void)
{
    return isReloading;
}

bool ACoreCharacter::IsWeaponInspecting(void)
{
    return isInspectingWeapon;
}

bool ACoreCharacter::IsRunning(void)
{
    return isRunning;
}