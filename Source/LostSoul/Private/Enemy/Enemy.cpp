// Developer Dmytro Alokhin

#include "Enemy/Enemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Navigation/PathFollowingComponent.h"
#include "Soul/Soul.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
    HealthBarWidget->SetupAttachment(GetRootComponent());

    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
    PawnSensing->SightRadius = 4000.0f;
    PawnSensing->SetPeripheralVisionAngle(45.0f);
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
    }
    InitializeEnemy();
    Tags.Add(FName("Enemy"));
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
    if (Target == nullptr) return false;

    const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
    return DistanceToTarget <= Radius;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
    const bool bShouldChaseTarget = !IsDead() &&                                                                                 //
                                    EnemyState != EEnemyState::EES_Chasing &&                                                    //
                                    EnemyState < EEnemyState::EES_Chasing &&                                                     //
                                    SeenPawn->ActorHasTag(FName("LostSoulCharacter")) && !SeenPawn->ActorHasTag(FName("Dead"));  //

    if (bShouldChaseTarget)
    {
        CombatTarget = SeenPawn;
        ClearPatrolTimer();
        ChaseTarget();
    }
}

void AEnemy::Attack()
{
    Super::Attack();
    if (CombatTarget == nullptr) return;

    EnemyState = EEnemyState::EES_Engaged;
    PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
    return IsInsideAttackRadius() && !IsAttacking() && !IsEngaged() && !IsDead();
}

void AEnemy::HandleDamage(float DamageAmount)
{
    Super::HandleDamage(DamageAmount);

    if (Attributes && HealthBarWidget)
    {
        HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
    }
}

void AEnemy::Die()
{
    Super::Die();

    EnemyState = EEnemyState::EES_Dead;
    ClearAttackTimer();
    HideHealthBar();
    DisableCapsule();
    SetLifeSpan(DeathLifeSpan);
    GetCharacterMovement()->bOrientRotationToMovement = false;
    SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
    SpawnSoul();
}

void AEnemy::SpawnSoul()
{
    UWorld* World = GetWorld();
    if (World && SoulClass && Attributes)
    {
        ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
        if (SpawnedSoul)
        {
            SpawnedSoul->SetSouls(Attributes->GetSouls());
        }
    }
}

void AEnemy::AttackEnd()
{
    EnemyState = EEnemyState::EES_NoState;
    CheckCombatTarget();
}

void AEnemy::ClearAttackTimer()
{
    GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::PatrolTimerFinished()
{
    MoveToTarget(PatrolTarget);
}

void AEnemy::MoveToTarget(AActor* Target)
{
    if (EnemyController == nullptr || Target == nullptr) return;
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(Target);
    MoveRequest.SetAcceptanceRadius(55.f);
    EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
    TArray<AActor*> ValidTargets;
    for (AActor* Target : PatrolTargets)
    {
        if (Target != PatrolTarget)
        {
            ValidTargets.AddUnique(Target);
        }
    }

    const int32 NumPatrolTargets = ValidTargets.Num();
    if (NumPatrolTargets > 0)
    {
        const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
        return ValidTargets[TargetSelection];
    }
    return nullptr;
}

void AEnemy::InitializeEnemy()
{
    EnemyController = Cast<AAIController>(GetController());
    HideHealthBar();
    MoveToTarget(PatrolTarget);
    SpawnDefaultWeapon();
}

void AEnemy::SpawnDefaultWeapon()
{
    UWorld* World = GetWorld();
    if (World && WeaponClass)
    {
        ABaseWeapon* DefaultWeapon = World->SpawnActor<ABaseWeapon>(WeaponClass);
        if (DefaultWeapon)
        {
            DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
            EquippedWeapon = DefaultWeapon;
        }
    }
}

void AEnemy::HideHealthBar()
{
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(false);
    }
}

void AEnemy::ShowHealthBar()
{
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(true);
    }
}

void AEnemy::LoseInterest()
{
    CombatTarget = nullptr;
    HideHealthBar();
}

void AEnemy::StartPatrolling()
{
    EnemyState = EEnemyState::EES_Patrolling;
    GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
    MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
    EnemyState = EEnemyState::EES_Chasing;
    GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
    MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
    return !InTargetRange(CombatTarget, CombatRadius);
}

void AEnemy::ClearPatrolTimer()
{
    GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool AEnemy::IsOutsideAttackRadius()
{
    return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
    return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsInsideAttackRadius()
{
    return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsAttacking()
{
    return EnemyState == EEnemyState::EES_Attacking;
}

void AEnemy::StartAttackTimer()
{
    EnemyState = EEnemyState::EES_Attacking;
    const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
    GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

bool AEnemy::IsDead()
{
    return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
    return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsDead()) return;
    if (EnemyState > EEnemyState::EES_Patrolling)
    {
        CheckCombatTarget();
    }
    else
    {
        CheckPatrolTarget();
    }
}

void AEnemy::CheckPatrolTarget()
{
    if (InTargetRange(PatrolTarget, PatrolRadius))
    {
        PatrolTarget = ChoosePatrolTarget();
        const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
        GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
    }
}

void AEnemy::CheckCombatTarget()
{
    if (IsOutsideCombatRadius())
    {
        ClearAttackTimer();
        LoseInterest();
        if (!IsEngaged())
        {
            StartPatrolling();
        }
    }
    else if (IsOutsideAttackRadius() && !IsChasing())
    {
        ClearAttackTimer();
        if (!IsEngaged())
        {
            ChaseTarget();
        }
    }
    else if (CanAttack())
    {
        StartAttackTimer();
    }
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
    Super::GetHit_Implementation(ImpactPoint, Hitter);
    if (!IsDead()) ShowHealthBar();
    ClearPatrolTimer();
    ClearAttackTimer();

    SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

    StopAttackMontage();
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    HandleDamage(Damage);
    CombatTarget = DamageCauser;

    if (IsInsideAttackRadius())
    {
        EnemyState = EEnemyState::EES_Attacking;
    }
    else if (IsOutsideAttackRadius())
    {
        ChaseTarget();
    }

    return Damage;
}

void AEnemy::Destroyed()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->Destroy();
    }
}
