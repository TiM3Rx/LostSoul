// Developer Dmytro Alokhin

#include "Weapon/BaseWeapon.h"
#include "Components/SphereComponent.h"

ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);

    Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    Sphere->SetupAttachment(GetRootComponent());
}

void ABaseWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    const FString OtherActorName = OtherActor->GetName();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
    }
}

void ABaseWeapon::OnSphereEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    const FString OtherActorName = FString("Ending Overlap with: ") + OtherActor->GetName();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, OtherActorName);
    }
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnSphereOverlap);
    Sphere->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnSphereEndOverlap);
}

void ABaseWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
