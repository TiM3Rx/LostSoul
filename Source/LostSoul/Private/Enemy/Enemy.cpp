// Developer Dmytro Alokhin

#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/HealthBarComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Navigation/PathFollowingComponent.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

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
    if (!HealthBarWidget) return;
    HealthBarWidget->SetVisibility(false);

    EnemyController = Cast<AAIController>(GetController());
    MoveToTarget(PatrolTarget);

    if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
    }

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

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
    if (Target == nullptr) return false;

    const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
    // if (GetWorld()) DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 25.f, 12, FColor::Red, false, -1.f);
    return DistanceToTarget <= Radius;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
    if (EnemyState == EEnemyState::EES_Chasing) return;
    if (SeenPawn->ActorHasTag(FName("LostSoulCharacter")))
    {
        GetWorldTimerManager().ClearTimer(PatrolTimer);
        GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
        CombatTarget = SeenPawn;

        if (EnemyState != EEnemyState::EES_Attacking)
        {
            EnemyState = EEnemyState::EES_Chasing;
            MoveToTarget(CombatTarget);
        }
    }
}

void AEnemy::Attack()
{
    Super::Attack();

    PlayAttackMontage();

}

void AEnemy::PlayAttackMontage()
{
    Super::PlayAttackMontage();

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance && !AttackMontage) return;

    AnimInstance->Montage_Play(AttackMontage);
    const int32 Selection = FMath::RandRange(0, 2);
    FName SectionName = FName();

    switch (Selection)
    {
        case 0: SectionName = FName("Attack1"); break;
        case 1: SectionName = FName("Attack2"); break;
        case 2: SectionName = FName("Attack3"); break;
    }
    AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);

}

void AEnemy::Die()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && DeathMontage)
    {
        AnimInstance->Montage_Play(DeathMontage);
        const int32 Selection = FMath::RandRange(0, 5);
        FName SectionName = FName();

        switch (Selection)
        {
            case 0:
                SectionName = FName("Death1");
                DeathPose = EDeathPose::EAS_Death1;
                break;
            case 1:
                SectionName = FName("Death2");
                DeathPose = EDeathPose::EAS_Death2;
                break;
            case 2:
                SectionName = FName("Death3");
                DeathPose = EDeathPose::EAS_Death3;
                break;
            case 3:
                SectionName = FName("Death4");
                DeathPose = EDeathPose::EAS_Death4;
                break;
            case 4:
                SectionName = FName("Death5");
                DeathPose = EDeathPose::EAS_Death5;
                break;
            case 5:
                SectionName = FName("Death6");
                DeathPose = EDeathPose::EAS_Death6;
                break;
        }
        AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
    }
    if (!HealthBarWidget) return;
    HealthBarWidget->SetVisibility(false);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetLifeSpan(5.0f);
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

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

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
    if (!InTargetRange(CombatTarget, CombatRadius))
    {
        CombatTarget = nullptr;
        if (HealthBarWidget)
        {
            HealthBarWidget->SetVisibility(false);
        }
        EnemyState = EEnemyState::EES_Patrolling;
        GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
        MoveToTarget(PatrolTarget);
    }
    else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
    {
        EnemyState = EEnemyState::EES_Chasing;
        GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
        MoveToTarget(CombatTarget);
    }
    else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
    {
        EnemyState = EEnemyState::EES_Attacking;
        Attack();
    }
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
    // DrawDebugSphere(GetWorld(), ImpactPoint, 8.f, 12, FColor::Blue, false, 5.f);
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(true);
    }

    if (Attributes && Attributes->IsAlive())
    {
        DirectionalHitReact(ImpactPoint);
    }
    else
    {
        Die();
    }

    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this,  //
            HitSound,                                //
            ImpactPoint);
    }
    if (HitParticles && GetWorld())
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),  //
            HitParticles,                                     //
            ImpactPoint);
    }
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (Attributes)
    {
        Attributes->ReceiveDamage(Damage);
        if (HealthBarWidget)
        {
            HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
        }
    }

    CombatTarget = DamageCauser;
    EnemyState = EEnemyState::EES_Chasing;
    GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
    MoveToTarget(CombatTarget);
    return Damage;
}

void AEnemy::Destroyed()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->Destroy();
    }
}
