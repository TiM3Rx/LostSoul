// Developer Dmytro Alokhin

#include "Chest/Chest.h"
#include "Components/SphereComponent.h"

AChest::AChest()
{
    PrimaryActorTick.bCanEverTick = true;

    Sphere = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    Sphere->SetupAttachment(RootComponent);

    SetRootComponent(Sphere);

    ChestBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshBodyComponent");
    ChestBodyMesh->SetupAttachment(RootComponent);

    ChestLidMesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshLidComponent");
    ChestLidMesh->SetupAttachment(RootComponent);
}

void AChest::BeginPlay()
{
    Super::BeginPlay();
}

void AChest::Interact()
{

}
