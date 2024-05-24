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

    void Equip(USceneComponent* Parent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);

    FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* Sphere;

    UFUNCTION()
    virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintImplementableEvent)
    void CreateFields(const FVector& FieldLocation);

    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "WeaponProperties")
    USoundBase* EquipSound;

    UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
    UBoxComponent* WeaponBox;

    UPROPERTY(EditAnywhere, Category = "WeaponProperties")
    float Damage = 20.0f;

    UPROPERTY(EditAnywhere, Category = "WeaponProperties")
    FVector BoxTraceExtent = FVector(5.f);

    UPROPERTY(EditAnywhere, Category = "WeaponProperties")
    bool bShowBoxDebug = false;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceStart;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceEnd;

    EWeaponState WeaponState = EWeaponState::EWS_None;

    void PlayEquipSound();
    void DisableSphereCollision();
    void ExecuteGetHit(FHitResult& BoxHit);
    void BoxTrace(FHitResult& BoxHit);

    bool ActorIsSameType(AActor* OtherActor);

};
