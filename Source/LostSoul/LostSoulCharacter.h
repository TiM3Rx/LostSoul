// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Logging/LogMacros.h"
#include "Character/CharacterType.h"
#include "LostSoulCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class AChest;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class ALostSoulCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    ALostSoulCharacter();

    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* InteractAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* AttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* EquipAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
    TArray<AChest*> Chests;

    UPROPERTY(BlueprintReadOnly, Category = "Interact")
    AChest* LastInteractedChest;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Interact(const FInputActionValue& Value);
    void Equip(const FInputActionValue& Value);
    void Attacking(const FInputActionValue& Value);
    virtual void Attack() override;

    void PlayEquipMontage(const FName SectionName);
    virtual void PlayAttackMontage() override;

    virtual void AttackEnd() override;

    UFUNCTION(BlueprintCallable)
    void Disarm();

    UFUNCTION(BlueprintCallable)
    void Arm();
    
    UFUNCTION(BlueprintCallable)
    void FinishEquipping();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* EquipMontage;

    AActor* GetInteractableObject() const;

    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EActionState ActionState = EActionState::EAS_Unoccupied;

    bool CanDisarm();
    bool CanArm();
    virtual bool CanAttack() override;
};
