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

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    ABaseWeapon* EquippedWeapon;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UAttributeComponent* Attributes;

    UFUNCTION(BlueprintCallable)
    virtual void AttackEnd();

    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

    virtual void Attack();
    virtual void Die();
    virtual void HandleDamage(float DamageAmount);
    virtual bool CanAttack();

    void DirectionalHitReact(const FVector& ImpactPoint);
    void PlayHitReactMontage(const FName& SectionName);
    void PlayHitSound(const FVector& ImpactPoint);
    void PawnHitParticles(const FVector& ImpactPoint);
    void DisableCapsule();

    virtual int32 PlayAttackMontage();
    virtual int32 PlayDeathMontage();

    bool IsAlive();

private:

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> AttackMontageSections;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> DeathMontageSections;

    UPROPERTY(EditAnywhere, Category = "Sounds")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UParticleSystem* HitParticles;

    void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

    int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
};
