// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

class ABaseWeapon;
class UAnimMontage;
class UAttributeComponent;
struct FInputActionValue;

UCLASS()
class LOSTSOUL_API ABaseCharacter : public ACharacter, public IHitInterface
{
    GENERATED_BODY()

public:
    ABaseCharacter();
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);


protected:
    virtual void BeginPlay() override;

    virtual void Attack();
    virtual bool CanAttack();
    virtual void AttackEnd();
    void DirectionalHitReact(const FVector& ImpactPoint);

    virtual void PlayAttackMontage();
    void PlayHitReactMontage(const FName SectionName);

    virtual void Die();

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    ABaseWeapon* EquippedWeapon;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* DeathMontage;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UAttributeComponent* Attributes;

    UPROPERTY(EditAnywhere, Category = "Sounds")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UParticleSystem* HitParticles;
  
};
