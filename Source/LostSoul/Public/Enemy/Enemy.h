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

    void CheckPatrolTarget();

    void CheckCombatTarget();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

    virtual float TakeDamage(float Damage,       //
        struct FDamageEvent const& DamageEvent,  //
        AController* EventInstigator,            //
        AActor* DamageCauser) override;

    virtual void Destroyed() override;


protected:
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly)
    EDeathPose DeathPose = EDeathPose::EDP_Alive;

    UPROPERTY(EditAnywhere)
    double PatrolRadius = 200.0f;

    UPROPERTY(EditAnywhere, Category = "EnemyMovement")
    float ChasingSpeed = 300.0f;

    UPROPERTY(EditAnywhere, Category = "EnemyMovement")
    float PatrollingSpeed = 125.0f;

    bool InTargetRange(AActor* Target, double Radius);

    UFUNCTION()
    void PawnSeen(APawn* SeenPawn);

    virtual void Attack() override;
    virtual void PlayAttackMontage() override;

    virtual void Die() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UHealthBarComponent* HealthBarWidget;

    UPROPERTY(EditAnywhere)
    double CombatRadius = 1000.0f;

    UPROPERTY(EditAnywhere)
    double AttackRadius = 150.0f;

    UPROPERTY()
    AActor* CombatTarget;

    UPROPERTY()
    class AAIController* EnemyController;

    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    AActor* PatrolTarget; 

    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    TArray<AActor*> PatrolTargets; 

    UPROPERTY(VisibleAnywhere)
    UPawnSensingComponent* PawnSensing;

    FTimerHandle PatrolTimer;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMin = 5.0f;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMax = 10.f;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class ABaseWeapon> WeaponClass;

    EEnemyState EnemyState = EEnemyState::EES_Patrolling;

    void PatrolTimerFinished();
    void MoveToTarget(AActor* Target);
    AActor* ChoosePatrolTarget();
};
