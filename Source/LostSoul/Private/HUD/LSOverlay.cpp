// Developer Dmytro Alokhin

#include "HUD/LSOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULSOverlay::SetHealthBarPercent(float Percent)
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetPercent(Percent);
    }
}

void ULSOverlay::SetStaminaBarPercent(float Percent)
{
    if (StaminaProgressBar)
    {
        StaminaProgressBar->SetPercent(Percent);
    }
}

void ULSOverlay::SetSouls(int32 Souls)
{
    if (SoulsText)
    {
        const FString String = FString::Printf(TEXT("%d"), Souls);
        const FText Text = FText::FromString(String);
        SoulsText->SetText(Text);
    }
}
