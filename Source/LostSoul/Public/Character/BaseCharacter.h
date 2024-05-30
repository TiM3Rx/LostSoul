// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Character/CharacterType.h"
#include "BaseCharacter.generated.h"

class ABaseWeapon;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class LOSTSOUL_API ABaseCharacter : public ACharacter, public IHitInterface
{
    GENERATED_BODY()

public:
    ABaseCharacter();
    virtual void Tick(float DeltaTime) override;

    FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    ABaseWeapon* EquippedWeapon;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UAttributeComponent* Attributes;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    AActor* CombatTarget;

    UPROPERTY(EditAnywhere, Category = "Combat")
    double WarpTargetDistance = 75.0f;

    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EDeathPose> DeathPose;

    UFUNCTION(BlueprintCallable)
    virtual void AttackEnd();

    UFUNCTION(BlueprintCallable)
    virtual void DodgeEnd();

    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

    UFUNCTION(BlueprintCallable)
    FVector GetTranslationWardTarget();

    UFUNCTION(BlueprintCallable)
    FVector GetRotationWardTarget();

    virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

    virtual void Attack();
    virtual void Die();
    virtual void HandleDamage(float DamageAmount);
    virtual bool CanAttack();
    virtual void PlayDodgeMontage();

    void DirectionalHitReact(const FVector& ImpactPoint);
    void PlayHitReactMontage(const FName& SectionName);
    void PlayHitSound(const FVector& ImpactPoint);
    void PawnHitParticles(const FVector& ImpactPoint);
    void DisableCapsule();
    void DisableMeshCollision();

    virtual int32 PlayAttackMontage();
    virtual int32 PlayDeathMontage();
    void StopAttackMontage();

    bool IsAlive();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* DodgeMontage;

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
