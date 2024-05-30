// Developer Dmytro Alokhin

#include "Soul/Soul.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ASoul::ASoul()
{
    PrimaryActorTick.bCanEverTick = true;

    SoulMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SoulMeshComponent"));
    SoulMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    SoulMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = SoulMesh;

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(GetRootComponent());
    Sphere->SetCollisionResponseToAllChannels(ECR_Overlap);
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASoul::OnSphereOverlap);

    ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
    ItemEffect->SetupAttachment(GetRootComponent());
}

void ASoul::BeginPlay()
{
    Super::BeginPlay();
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
    if (PickupInterface)
    {
        PickupInterface->AddSouls(this);
        SpawnVFXPickupEffect();
        SpawnSoundPickup();

        Destroy();
    }
}

void ASoul::SpawnVFXPickupEffect()
{
    if (PickupEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,  //
            PickupEffect,                                     //
            GetActorLocation()                                //
        );
    }
}

void ASoul::SpawnSoundPickup()
{
    if (PickupSound)
    {
        UGameplayStatics::SpawnSoundAtLocation(this,  //
            PickupSound,                              //
            GetActorLocation()                        //
        );
    }
}

void ASoul::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
