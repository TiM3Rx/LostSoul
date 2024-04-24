// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USphereComponent;
class USoundBase;
class UBoxComponent;

enum class EWeaponState : uint8
{
    EWS_None,
    EWS_Equipped
};

UCLASS()
class LOSTSOUL_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseWeapon();

    virtual void Tick(float DeltaTime) override;

    TArray<AActor*> IgnoreActors;

    FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* Sphere;

    UFUNCTION()
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnSphereEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    virtual void BeginPlay() override;

    void Equip();

private:
    UPROPERTY(EditAnywhere, Category = "WeaponProperties")
    USoundBase* EquipSound;

    UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
    UBoxComponent* WeaponBox;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceStart;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceEnd;

    EWeaponState WeaponState = EWeaponState::EWS_None;
};
