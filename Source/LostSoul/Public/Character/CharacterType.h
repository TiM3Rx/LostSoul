#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    ECS_Unequipped UMETA(DisplayName = "Unequipped"),
    ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
    EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
    EAS_Attacking UMETA(DisplayName = "Attacking"),
    EAS_EquippingWeapon UMETA(DisplayName = "EquippingWeapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
    EDP_Alive UMETA(DisplayName = "Alive"),
    EAS_Death1 UMETA(DisplayName = "Death1"),
    EAS_Death2 UMETA(DisplayName = "Death2"),
    EAS_Death3 UMETA(DisplayName = "Death3"),
    EAS_Death4 UMETA(DisplayName = "Death4"),
    EAS_Death5 UMETA(DisplayName = "Death5"),
    EAS_Death6 UMETA(DisplayName = "Death6")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    EES_Patrolling UMETA(DisplayName = "Patrolling"),
    EES_Chasing UMETA(DisplayName = "Chasing"),
    EES_Attacking UMETA(DisplayName = "Atacking")

};