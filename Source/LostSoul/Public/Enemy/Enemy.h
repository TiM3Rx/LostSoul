// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Character/CharacterType.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;

UCLASS()
class LOSTSOUL_API AEnemy : public ACharacter, public IHitInterface
{
    GENERATED_BODY()

public:
    AEnemy();
    virtual void Tick(float DeltaTime) override;

    void CheckPatrolTarget();

    void CheckCombatTarget();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

    virtual float TakeDamage(float Damage,       //
        struct FDamageEvent const& DamageEvent,  //
        AController* EventInstigator,            //
        AActor* DamageCauser) override;


protected:
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly)
    EDeathPose DeathPose = EDeathPose::EDP_Alive;

    UPROPERTY(EditAnywhere)
    double PatrolRadius = 200.0f;

    bool InTargetRange(AActor* Target, double Radius);

    void Die();

    void PlayHitReactMontage(const FName SectionName);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditAnywhere, Category = "Sounds")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UParticleSystem* HitParticles;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UAttributeComponent* Attributes;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UHealthBarComponent* HealthBarWidget;

    UPROPERTY(EditAnywhere)
    double CombatRadius = 1000.0f;

    UPROPERTY()
    AActor* CombatTarget;

    UPROPERTY()
    class AAIController* EnemyController;

    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    AActor* PatrolTarget; 

    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    TArray<AActor*> PatrolTargets; 

    FTimerHandle PatrolTimer;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMin = 5.0f;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMax = 10.f;

    void PatrolTimerFinished();
    void MoveToTarget(AActor* Target);
    AActor* ChoosePatrolTarget();
    void DirectionalHitReact(const FVector& ImpactPoint);
};
