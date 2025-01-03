// Developer Dmytro Alokhin

#include "Chest/Chest.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LostSoul/LostSoulCharacter.h"
#include "Weapon/BaseWeapon.h"

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

    TArray<AActor*> ChestActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChest::StaticClass(), ChestActors);

    ChestsOpened.Init(false, ChestActors.Num());
}

void AChest::Interaction()
{
    int32 ChestIndex = ChestsOpened.Find(false);

    if (ChestIndex != INDEX_NONE)
    {
        OpenChest();
        ChestsOpened[ChestIndex] = true;
        bIsOpened = true;
    }
}

void AChest::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsInteracted)
    {
        RunningTime += DeltaTime;
        SetLidRotation();
    }
}


void AChest::OpenChest()
{
    IsInteracted = true;
    RunningTime = 0.0f;
    GetWorldTimerManager().SetTimer(OpenTimerHandle, this, &AChest::SetLidRotation, OpenDuration, true);

    if (bIsOpened)
    {
        SpawnWeapon();
    }
}

void AChest::SpawnWeapon()
{
    if (WeaponToSpawn)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        ABaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
        SpawnedWeapon = NewWeapon;

        ALostSoulCharacter* LostSoulCharacter =
            Cast<ALostSoulCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (LostSoulCharacter && NewWeapon)
        {
            NewWeapon->AttachToComponent(LostSoulCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHandSocket");
        }
    }
}

void AChest::SetLidRotation() 
{
    const float CurrentRotation = ChestLidMesh->GetRelativeRotation().Yaw;
    const float LerpedRotation = FMath::Lerp(CurrentRotation, TargetRotation, RunningTime / OpenDuration);

    FRotator NewRotation = FRotator(0.0f, 0.0f, LerpedRotation);
    ChestLidMesh->SetRelativeRotation(NewRotation);

    if (RunningTime >= OpenDuration)
    {
        IsInteracted = false;
        GetWorldTimerManager().ClearTimer(OpenTimerHandle);
    }
}