// Developer Dmytro Alokhin


#include "Weapon/SwordWeapon.h"
#include "LostSoul/LostSoulCharacter.h"

void ASwordWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    //ALostSoulCharacter* LostSoulCharacter = Cast<ALostSoulCharacter>(OtherActor);

    //if (!LostSoulCharacter) return;

    ////FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    ////WeaponMesh->AttachToComponent(LostSoulCharacter->GetMesh(), TransformRules, FName("RightHandSocket"));
}

void ASwordWeapon::OnSphereEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
