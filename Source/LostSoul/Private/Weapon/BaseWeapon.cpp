// Developer Dmytro Alokhin

#include "Weapon/BaseWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
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

void ABaseWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ActorIsSameType(OtherActor)) return;

    FHitResult BoxHit;
    BoxTrace(BoxHit);

    if (BoxHit.GetActor())
    {
        if (ActorIsSameType(BoxHit.GetActor())) return;
        UGameplayStatics::ApplyDamage(BoxHit.GetActor(),  //
            Damage,                                       //
            GetInstigatorController(),                    //
            this,                                         //
            UDamageType::StaticClass());

        ExecuteGetHit(BoxHit);
        
        CreateFields(BoxHit.ImpactPoint);
    }
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnBoxOverlap);
}

void ABaseWeapon::BoxTrace(FHitResult& BoxHit)
{
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    for (AActor* Actor : IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    UKismetSystemLibrary::BoxTraceSingle(this,                                 //
        Start,                                                                 //
        End,                                                                   //
        BoxTraceExtent,                                                        //
        BoxTraceStart->GetComponentRotation(),                                 //
        ETraceTypeQuery::TraceTypeQuery1,                                      //
        false, ActorsToIgnore,                                                 //
        bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,  //
        BoxHit,                                                                //
        true);

    IgnoreActors.AddUnique(BoxHit.GetActor());
}

void ABaseWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
    IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
    if (HitInterface)
    {
        HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
    }
}

void ABaseWeapon::DisableSphereCollision()
{
    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

bool ABaseWeapon::ActorIsSameType(AActor* OtherActor)
{
    return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
   
}

void ABaseWeapon::PlayEquipSound()
{
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
    }
}

void ABaseWeapon::Equip(USceneComponent* Parent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
    WeaponState = EWeaponState::EWS_Equipped;
    PlayEquipSound();
    DisableSphereCollision();

    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(Parent, TransformRules, SocketName);

    SetOwner(NewOwner);
    SetInstigator(NewInstigator);
}

void ABaseWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
