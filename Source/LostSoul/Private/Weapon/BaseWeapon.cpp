// Developer Dmytro Alokhin

#include "Weapon/BaseWeapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Interface/HitInterface.h"

ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);

    Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    Sphere->SetupAttachment(GetRootComponent());

    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
    WeaponBox->SetupAttachment(GetRootComponent());
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
    BoxTraceStart->SetupAttachment(GetRootComponent());

    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void ABaseWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    /*const FString OtherActorName = OtherActor->GetName();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
    }*/
}

void ABaseWeapon::OnSphereEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    /*const FString OtherActorName = FString("Ending Overlap with: ") + OtherActor->GetName();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, OtherActorName);
    }*/
}

void ABaseWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    for (AActor* Actor : IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    FHitResult BoxHit;
    UKismetSystemLibrary::BoxTraceSingle(this,  //
        Start,                                  //
        End,                                    //
        FVector(5.f, 5.f, 5.f),                 //
        BoxTraceStart->GetComponentRotation(),  //
        ETraceTypeQuery::TraceTypeQuery1,       //
        false, ActorsToIgnore,                  //
        EDrawDebugTrace::None,                  //
        BoxHit,                                 //
        true);
    if (BoxHit.GetActor())
    {
        IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
        if (HitInterface)
        {
            HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
        }
        IgnoreActors.AddUnique(BoxHit.GetActor());

        CreateFields(BoxHit.ImpactPoint);
    }
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnSphereOverlap);
    Sphere->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnSphereEndOverlap);

    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnBoxOverlap);
}

void ABaseWeapon::Equip()
{
    WeaponState = EWeaponState::EWS_Equipped;
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
    }
    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void ABaseWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
