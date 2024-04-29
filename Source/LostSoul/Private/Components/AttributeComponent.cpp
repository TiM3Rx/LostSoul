// Developer Dmytro Alokhin

#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UAttributeComponent::IsAlive()
{
    return Health > 0.0f;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
    return Health / MaxHealth;
}
