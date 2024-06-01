// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "Door.generated.h"

class UBoxComponent;

UCLASS()
class LOSTSOUL_API ADoor : public AActor, public IInteractInterface
{
    GENERATED_BODY()

public:
    ADoor();
    virtual void Tick(float DeltaTime) override;
    virtual void Interaction() override;

protected:
    virtual void BeginPlay() override;

private:
    void OpenDoor();
    void RotateDoor();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Door;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* DoorFrame;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
    float OpenDuration = 2.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door", meta = (AllowPrivateAccess = "true"))
    float TargetRotation = -70.0f;

    bool bIsOpened = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float RunningTime = 0.0f;

    FTimerHandle OpenTimerHandle;
};
