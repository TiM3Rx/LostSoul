// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterType.h"
#include "LSAnimInstance.generated.h"


UCLASS()
class LOSTSOUL_API ULSAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaTime) override;

    UPROPERTY(BlueprintReadOnly)
    class ALostSoulCharacter* LostSoulCharacter;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    class UCharacterMovementComponent* LostSoulCharacterMovement;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float GroundSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool IsFalling;

    UPROPERTY(BlueprintReadOnly, Category = "Character State")
    ECharacterState CharacterState;

    UPROPERTY(BlueprintReadOnly, Category = "Action State")
    EActionState ActionState;

    UPROPERTY(BlueprintReadOnly, Category = "Action State")
    TEnumAsByte<EDeathPose> DeathPose;

};
