// Developer Dmytro Alokhin

#include "Character/LSAnimInstance.h"
#include "LostSoul/LostSoulCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ULSAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    LostSoulCharacter = Cast<ALostSoulCharacter>(TryGetPawnOwner());
    if (LostSoulCharacter)
    {
        LostSoulCharacterMovement = LostSoulCharacter->GetCharacterMovement();
    }
}

void ULSAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (LostSoulCharacterMovement)
    {
        GroundSpeed = UKismetMathLibrary::VSizeXY(LostSoulCharacterMovement->Velocity);
        IsFalling = LostSoulCharacterMovement->IsFalling();
        CharacterState = LostSoulCharacter->GetCharacterState();
    }
}
