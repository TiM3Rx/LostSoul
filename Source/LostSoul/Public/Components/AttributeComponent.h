// Developer Dmytro Alokhin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOSTSOUL_API UAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAttributeComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void RegenStamina(float DeltaTime);

    void ReceiveDamage(float Damage);
    void UseStamina(float StaminaCost);
    float GetHealthPercent();
    float GetStaminaPercent();
    bool IsAlive();

    void AddSouls(int32 NumberOfSouls);

    FORCEINLINE int32 GetSouls() const { return Souls; }
    FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
    FORCEINLINE float GetStamina() const { return Stamina; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float Health;

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float MaxHealth;

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float Stamina;

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float MaxStamina;

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float DodgeCost = 30.0f;

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float StaminaRegenRate = 8.0f;

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    int32 Souls;
};
