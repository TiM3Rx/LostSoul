// Developer Dmytro Alokhin

#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Weapon/BaseWeapon.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseCharacter::Attack() {}

void ABaseCharacter::PlayAttackMontage() {}

void ABaseCharacter::PlayHitReactMontage(const FName SectionName)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && HitReactMontage)
    {
        AnimInstance->Montage_Play(HitReactMontage);
        AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
    }
}

bool ABaseCharacter::CanAttack()
{
    return false;
}

void ABaseCharacter::AttackEnd() {}

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

    /*if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
    }*/

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

    // UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.0f, 4.0f, FColor::Red, 5.0f);
    // UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.0f, 4.0f, FColor::Green, 5.0f);
    // UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.0f, 4.0f,
    // FColor::Blue, 5.0f);
}

void ABaseCharacter::Die() {}

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

