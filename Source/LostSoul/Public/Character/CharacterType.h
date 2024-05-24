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
enum EDeathPose
{
    EDS_Death1 UMETA(DisplayName = "Death1"),
    EDS_Death2 UMETA(DisplayName = "Death2"),
    EDS_Death3 UMETA(DisplayName = "Death3"),
    EDS_Death4 UMETA(DisplayName = "Death4"),
    EDS_Death5 UMETA(DisplayName = "Death5"),
    EDS_Death6 UMETA(DisplayName = "Death6"),

    EDP_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    EES_Dead UMETA(DisplayName = "Dead"),
    EES_Patrolling UMETA(DisplayName = "Patrolling"),
    EES_Chasing UMETA(DisplayName = "Chasing"),
    EES_Attacking UMETA(DisplayName = "Atacking"),
    EES_Engaged UMETA(DisplayName = "Engaged"),

    EES_NoState UMETA(DisplayName = "NoState")
};