// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
    GENERATED_BODY()
};

class LOSTSOUL_API IPickupInterface
{
    GENERATED_BODY()

public:
    virtual void AddSouls(class ASoul* Soul);
};
