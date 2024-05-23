// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class LOSTSOUL_API ABreakableActor : public AActor, public IHitInterface
{
    GENERATED_BODY()

public:
    ABreakableActor();

    virtual void Tick(float DeltaTime) override;

    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UGeometryCollectionComponent* GeometryCollection;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UCapsuleComponent* Capsule;

private:
    UPROPERTY(VisibleAnywhere, Category = "Pots")
    int32 NumsOfPots = 3; 

    bool bBroken = false;
};
