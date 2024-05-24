// Developer Dmytro Alokhin

#include "LostSoulCharacter.h"
#include "Interface/InteractInterface.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Chest/Chest.h"
#include "Animation/AnimMontage.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ALostSoulCharacter::ALostSoulCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
    GetMesh()->SetGenerateOverlapEvents(true);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void ALostSoulCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
    PlayHitSound(ImpactPoint);
    PawnHitParticles(ImpactPoint);
}

void ALostSoulCharacter::BeginPlay()
{
    Super::BeginPlay();

    Tags.Add(FName("LostSoulCharacter"));

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

AActor* ALostSoulCharacter::GetInteractableObject() const
{
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
        {
            return Actor;
        }
    }

    return nullptr;
}

bool ALostSoulCharacter::CanAttack()
{
    return ActionState == EActionState::EAS_Unoccupied &&  //
           CharacterState != ECharacterState::ECS_Unequipped;
}

bool ALostSoulCharacter::CanDisarm()
{
    return ActionState == EActionState::EAS_Unoccupied &&  //
           CharacterState != ECharacterState::ECS_Unequipped;
}

bool ALostSoulCharacter::CanArm()
{
    return ActionState == EActionState::EAS_Unoccupied &&        //
           CharacterState == ECharacterState::ECS_Unequipped &&  //
           EquippedWeapon;
}

void ALostSoulCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {

        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALostSoulCharacter::Move);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALostSoulCharacter::Look);

        // Interact
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ALostSoulCharacter::Interact);

        // Attack
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ALostSoulCharacter::PerformAttack);

        // Equip
        EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ALostSoulCharacter::Equip);
    }
    else
    {
        UE_LOG(LogTemplateCharacter, Error,
            TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend "
                 "to use the legacy system, then you will need to update this C++ file."),
            *GetNameSafe(this));
    }
}

void ALostSoulCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (ActionState != EActionState::EAS_Unoccupied) return;
    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ALostSoulCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ALostSoulCharacter::Interact(const FInputActionValue& Value)
{
    if (ActionState == EActionState::EAS_Attacking) return;
    if (LastInteractedChest)
    {
        return;
    }

    AActor* InteractableObject = GetInteractableObject();

    if (InteractableObject && InteractableObject->IsA<AChest>())
    {
        AChest* InteractedChest = Cast<AChest>(InteractableObject);
        if (InteractedChest)
        {
            InteractedChest->Interaction();
            LastInteractedChest = InteractedChest;
            
            CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
            EquippedWeapon = InteractedChest->GetSpawnedWeapon();
        }
    }
    LastInteractedChest = nullptr;
}

void ALostSoulCharacter::Attack()
{
    Super::Attack();
    if (CanAttack())
    {
        PlayAttackMontage();
        ActionState = EActionState::EAS_Attacking;
    }
}

void ALostSoulCharacter::PerformAttack(const FInputActionValue& Value)
{
    Attack();
}

void ALostSoulCharacter::Equip(const FInputActionValue& Value)
{
    if (CanDisarm())
    {
        PlayEquipMontage(FName("Unequip"));
        CharacterState = ECharacterState::ECS_Unequipped;
        ActionState = EActionState::EAS_EquippingWeapon;
    }
    else if (CanArm())
    {
        PlayEquipMontage(FName("Equip"));
        CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
        ActionState = EActionState::EAS_EquippingWeapon;
    }
}

void ALostSoulCharacter::PlayEquipMontage(const FName SectionName)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance && !EquipMontage) return;

    AnimInstance->Montage_Play(EquipMontage);
    AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
}

void ALostSoulCharacter::AttackEnd()
{
    ActionState = EActionState::EAS_Unoccupied;
}

void ALostSoulCharacter::Disarm()
{
    if (EquippedWeapon)
    {
        FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
        EquippedWeapon->AttachToComponent(GetMesh(), TransformRules, FName("SpineSocket"));                       
        CharacterState = ECharacterState::ECS_Unequipped;
    }
}

void ALostSoulCharacter::Arm()
{
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    EquippedWeapon->AttachToComponent(GetMesh(), TransformRules, FName("RightHandSocket"));
    CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
}

void ALostSoulCharacter::FinishEquipping()
{
    ActionState = EActionState::EAS_Unoccupied;
}
