// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class LOSTSOUL_API ULSOverlay : public UUserWidget
{
    GENERATED_BODY()
public:

    void SetHealthBarPercent(float Percent);
    void SetStaminaBarPercent(float Percent);
    void SetSouls(int32 Souls);

private:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* StaminaProgressBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SoulsText;

};
