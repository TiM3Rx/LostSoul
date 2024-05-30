// Developer Dmytro Alokhin

#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/CapsuleComponent.h"

ABreakableActor::ABreakableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
    GeometryCollection->SetGenerateOverlapEvents(true);
    GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    SetRootComponent(GeometryCollection);

    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    Capsule->SetupAttachment(GetRootComponent());
    Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
    Super::BeginPlay();
}

void ABreakableActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
    if (bBroken) return;
    bBroken = true;
    UWorld* World = GetWorld();
    if (World)
    {
        FVector Location = GetActorLocation();
        Location.Z += 75.f;

        World->SpawnActor<ABreakableActor>(Location, GetActorRotation());
    }
}
