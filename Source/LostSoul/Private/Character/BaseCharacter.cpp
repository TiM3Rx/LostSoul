// Developer Dmytro Alokhin

#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BaseWeapon.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseCharacter::Attack()
{
    if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
    {
        CombatTarget = nullptr;
    }
}

int32 ABaseCharacter::PlayAttackMontage()
{
    return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
    const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
    TEnumAsByte<EDeathPose> Pose(Selection);
    if (Pose < EDeathPose::EDP_MAX)
    {
        DeathPose = Pose;
    }

    return Selection;
}

void ABaseCharacter::StopAttackMontage()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Stop(0.25f, AttackMontage);
    }
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
    if (Attributes)
    {
        Attributes->ReceiveDamage(DamageAmount);
    }
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && HitReactMontage)
    {
        AnimInstance->Montage_Play(HitReactMontage);
        AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
    }
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this,  //
            HitSound,                                //
            ImpactPoint);
    }
}

void ABaseCharacter::PawnHitParticles(const FVector& ImpactPoint)
{
    if (HitParticles && GetWorld())
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),  //
            HitParticles,                                     //
            ImpactPoint);
    }
}

void ABaseCharacter::DisableCapsule()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::DisableMeshCollision()
{
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
    if (SectionNames.Num() <= 0) return -1;
    const int32 MaxSectionIndex = SectionNames.Num() - 1;
    const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
    PlayMontageSection(Montage, SectionNames[Selection]);
    return Selection;
}

bool ABaseCharacter::CanAttack()
{
    return false;
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && Montage)
    {
        AnimInstance->Montage_Play(Montage);
        AnimInstance->Montage_JumpToSection(SectionName, Montage);
    }
}

void ABaseCharacter::PlayDodgeMontage()
{
    PlayMontageSection(DodgeMontage, FName("Default"));
}

void ABaseCharacter::AttackEnd() {}

void ABaseCharacter::DodgeEnd() {}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
    const FVector Forward = GetActorForwardVector();
    const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
    const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
    const double CosTheta = FVector::DotProduct(Forward, ToHit);

    double Theta = FMath::Acos(CosTheta);
    Theta = FMath::RadiansToDegrees(Theta);

    const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
    if (CrossProduct.Z < 0)
    {
        Theta *= -1.0f;
    }

    FName Section("ReactFromBack");
    if (Theta >= -45.f && Theta < 45.f)
    {
        Section = FName("ReactFromFront");
    }
    else if (Theta >= -135.f && Theta < -45.f)
    {
        Section = FName("ReactFromLeft");
    }
    else if (Theta >= 45.f && Theta < 135.f)
    {
        Section = FName("ReactFromRight");
    }
    PlayHitReactMontage(Section);
}

void ABaseCharacter::Die()
{
    Tags.Add(FName("Dead"));
    PlayDeathMontage();
}

bool ABaseCharacter::IsAlive()
{
    return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
    if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
    {
        EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
        EquippedWeapon->IgnoreActors.Empty();
    }
}

FVector ABaseCharacter::GetTranslationWardTarget()
{
    if (CombatTarget == nullptr) return FVector();

    const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
    const FVector Location = GetActorLocation();

    FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
    TargetToMe *= WarpTargetDistance;

    return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWardTarget()
{
    if (CombatTarget)
    {
        return CombatTarget->GetActorLocation();
    }
    return FVector();
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
    if (IsAlive() && Hitter)
    {
        DirectionalHitReact(Hitter->GetActorLocation());
    }
    else
    {
        Die();
    }

    PlayHitSound(ImpactPoint);
    PawnHitParticles(ImpactPoint);
}
