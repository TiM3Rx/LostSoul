// Developer Dmytro Alokhin

#include "Door/Door.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

ADoor::ADoor()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    SetRootComponent(BoxCollision);

    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameComponent"));
    DoorFrame->SetupAttachment(RootComponent);

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorComponent"));
    Door->SetupAttachment(RootComponent);
}

void ADoor::BeginPlay()
{
    Super::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsOpened)
    {
        RunningTime += DeltaTime;
        RotateDoor();
    }
}

void ADoor::OpenDoor()
{
    RunningTime = 0.0f;
    bIsOpened = true;
    GetWorldTimerManager().SetTimer(OpenTimerHandle, this, &ADoor::RotateDoor, OpenDuration, true);
}

void ADoor::RotateDoor()
{
    float CurrentRotation = Door->GetRelativeRotation().Yaw;
    float LerpedRotation = FMath::Lerp(CurrentRotation, TargetRotation, RunningTime / OpenDuration);

    Door->SetRelativeRotation(FRotator(0.0f, LerpedRotation, 0.0f));

    if (RunningTime >= OpenDuration)
    {
        GetWorldTimerManager().ClearTimer(OpenTimerHandle);
    }
    BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADoor::Interaction()
{
    if (!bIsOpened)
    {
        OpenDoor();
    }
}
