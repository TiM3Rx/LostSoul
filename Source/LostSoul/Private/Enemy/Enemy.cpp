// Developer Dmytro Alokhin

#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/HealthBarComponent.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
    HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
    HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    if (!HealthBarWidget) return;
    HealthBarWidget->SetVisibility(false);
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

void AEnemy::PlayHitReactMontage(const FName SectionName)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && HitReactMontage)
    {
        AnimInstance->Montage_Play(HitReactMontage);
        AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
    }
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
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
    // UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.0f, 4.0f, FColor::Blue, 5.0f);
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CombatTarget)
    {
        const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
        if (DistanceToTarget > CombatRadius)
        {
            CombatTarget = nullptr;
            if (HealthBarWidget)
            {
                HealthBarWidget->SetVisibility(false);
            }
        }
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
    return Damage;
}
