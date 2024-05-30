// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Logging/LogMacros.h"
#include "Character/CharacterType.h"
#include "Interface/PickupInterface.h"
#include "LostSoulCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class ULSOverlay;
class AChest;
class ASoul;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class ALostSoulCharacter : public ABaseCharacter, public IPickupInterface
{
    GENERATED_BODY()

public:
    ALostSoulCharacter();
    virtual void Tick(float DeltaTime) override;

    virtual void AddSouls(ASoul* Soul) override;

    virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

    virtual float TakeDamage(float Damage,       //
        struct FDamageEvent const& DamageEvent,  //
        AController* EventInstigator,            //
        AActor* DamageCauser) override;

    virtual void Jump() override;

    FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
    FORCEINLINE EActionState GetActionState() const { return ActionState; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* InteractAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* AttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* EquipAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* DodgeAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
    TArray<AChest*> Chests;

    UPROPERTY(BlueprintReadOnly, Category = "Interact")
    AChest* LastInteractedChest;

    UFUNCTION(BlueprintCallable)
    void Disarm();

    UFUNCTION(BlueprintCallable)
    void Arm();

    UFUNCTION(BlueprintCallable)
    void FinishEquipping();

    UFUNCTION(BlueprintCallable)
    void HitReactEnd();

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Interact(const FInputActionValue& Value);
    void Equip(const FInputActionValue& Value);
    void PerformAttack(const FInputActionValue& Value);
    void Dodge(const FInputActionValue& Value);

    void PlayEquipMontage(const FName SectionName);

    bool HasEnoughStamina();
    bool IsOccupied();

    virtual void BeginPlay();
    virtual void Die() override;
    virtual void Attack() override;
    virtual void AttackEnd() override;
    virtual void DodgeEnd() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* EquipMontage;

    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EActionState ActionState = EActionState::EAS_Unoccupied;

    UPROPERTY()
    ULSOverlay* LSOverlay;

    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

    AActor* GetInteractableObject() const;

    void EnhancedInput(APlayerController* PlayerController);
    void InitializeLSOverlay(APlayerController* PlayerController);
    void SetHUDHealth();

    bool IsUnoccupied();
    bool CanDisarm();
    bool CanArm();

    virtual bool CanAttack() override;
};
