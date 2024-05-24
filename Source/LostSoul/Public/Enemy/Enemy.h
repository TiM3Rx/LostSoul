// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterType.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class LOSTSOUL_API AEnemy : public ABaseCharacter
{
    GENERATED_BODY()

public:
    AEnemy();
    virtual void Tick(float DeltaTime) override;

    virtual float TakeDamage(float Damage,       //
        struct FDamageEvent const& DamageEvent,  //
        AController* EventInstigator,            //
        AActor* DamageCauser) override;

    virtual void Destroyed() override;

    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EDeathPose> DeathPose;

    UPROPERTY(BlueprintReadOnly)
    EEnemyState EnemyState = EEnemyState::EES_Patrolling;

    virtual void Attack() override;
    virtual bool CanAttack() override;
    virtual void HandleDamage(float DamageAmount);
    virtual void Die() override;
    virtual void AttackEnd() override;
    virtual int32 PlayDeathMontage() override;

private:
    /* AI Behavior* */

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UHealthBarComponent* HealthBarWidget;

    UPROPERTY(VisibleAnywhere)
    UPawnSensingComponent* PawnSensing;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class ABaseWeapon> WeaponClass;

    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    AActor* PatrolTarget;

    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    TArray<AActor*> PatrolTargets;

    FTimerHandle PatrolTimer;

    UPROPERTY()
    class AAIController* EnemyController;

    UPROPERTY()
    AActor* CombatTarget;

    UPROPERTY(EditAnywhere)
    double CombatRadius = 1000.0f;

    UPROPERTY(EditAnywhere)
    double AttackRadius = 150.0f;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMin = 5.0f;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMax = 10.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackMin = 0.5f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackMax = 1.0f;

    UPROPERTY(EditAnywhere)
    double PatrolRadius = 200.0f;

    UPROPERTY(EditAnywhere, Category = "EnemyMovement")
    float ChasingSpeed = 300.0f;

    UPROPERTY(EditAnywhere, Category = "EnemyMovement")
    float PatrollingSpeed = 125.0f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float DeathLifeSpan = 5.0f;

    UFUNCTION()
    void PawnSeen(APawn* SeenPawn);

    FTimerHandle AttackTimer;
    AActor* ChoosePatrolTarget();

    void CheckPatrolTarget();
    void CheckCombatTarget();

    void InitializeEnemy();
    void SpawnDefaultWeapon();
    void HideHealthBar();
    void ShowHealthBar();
    void LoseInterest();
    void StartPatrolling();
    void ChaseTarget();

    void ClearPatrolTimer();

    void StartAttackTimer();
    void ClearAttackTimer();

    void PatrolTimerFinished();
    void MoveToTarget(AActor* Target);

    bool IsOutsideCombatRadius();
    bool IsOutsideAttackRadius();
    bool IsInsideAttackRadius();
    bool InTargetRange(AActor* Target, double Radius);
    bool IsChasing();
    bool IsAttacking();
    bool IsDead();
    bool IsEngaged();
};
