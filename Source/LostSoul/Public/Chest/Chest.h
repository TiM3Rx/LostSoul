// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "Chest.generated.h"

class USphereComponent;
class ABaseWeapon;

UCLASS()
class LOSTSOUL_API AChest : public AActor, public IInteractInterface
{
    GENERATED_BODY()

public:
    AChest();

    virtual void Interaction() override;
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* ChestBodyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* ChestLidMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* Sphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Chest")
    float OpenDuration = 2.5f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Chest")
    float TargetRotation = -70.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chest")
    TSubclassOf<ABaseWeapon> WeaponToSpawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool IsInteracted = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsOpened = false;


private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float RunningTime;

    FTimerHandle OpenTimerHandle;

    TArray<bool> ChestsOpened;

    void SetLidRotation();
    void OpenChest();
    void SpawnWeapon();
};
