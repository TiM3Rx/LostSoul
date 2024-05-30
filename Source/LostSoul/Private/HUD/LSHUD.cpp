// Developer Dmytro Alokhin

#include "HUD/LSHUD.h"
#include "HUD/LSOverlay.h"

void ALSHUD::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* Controller = World->GetFirstPlayerController();
        if (Controller && LSOverlayClass)
        {
            LSOverlay =  CreateWidget<ULSOverlay>(Controller, LSOverlayClass);
            LSOverlay->AddToViewport();
        }
    }
}
