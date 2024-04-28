// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
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

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
    virtual void BeginPlay() override;

    void PlayHitReactMontage(const FName SectionName);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditAnywhere, Category = "Sounds")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, Category = "VFX")
    UParticleSystem* HitParticles;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UAttributeComponent* Attributes;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UHealthBarComponent* HealthBarWidget;

    void DirectionalHitReact(const FVector& ImpactPoint);
};
