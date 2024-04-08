// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
    GENERATED_BODY()
};

class LOSTSOUL_API IInteractInterface
{
    GENERATED_BODY()

public:
    
    virtual void Interaction() = 0;

};
