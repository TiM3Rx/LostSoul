// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LSHUD.generated.h"

class ULSOverlay;

UCLASS()
class LOSTSOUL_API ALSHUD : public AHUD
{
    GENERATED_BODY()

public:

    FORCEINLINE ULSOverlay* GetLSOverlay() const { return LSOverlay; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "LostSoul")
    TSubclassOf<ULSOverlay> LSOverlayClass;

    UPROPERTY()
    ULSOverlay* LSOverlay;
};
