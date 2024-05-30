// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PickupInterface.h"
#include "Soul.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;
class USphereComponent;

UCLASS()
class LOSTSOUL_API ASoul : public AActor, public IPickupInterface
{
    GENERATED_BODY()

public:
    ASoul();
    virtual void Tick(float DeltaTime) override;

    FORCEINLINE int32 GetSouls() const { return Souls; }
    FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* SoulMesh;

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    USphereComponent* Sphere;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraComponent* ItemEffect;

    UFUNCTION()
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void SpawnVFXPickupEffect();
    void SpawnSoundPickup();

private:
    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraSystem* PickupEffect;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundBase* PickupSound;

    UPROPERTY(EditAnywhere)
    int32 Souls = 1;
};
