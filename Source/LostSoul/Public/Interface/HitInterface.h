// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
    GENERATED_BODY()
};

class LOSTSOUL_API IHitInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    void GetHit(const FVector& ImpactPoint); 
};