// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "AxeWeapon.generated.h"

UCLASS()
class LOSTSOUL_API AAxeWeapon : public ABaseWeapon
{
    GENERATED_BODY()

protected:
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
    virtual void OnSphereEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
