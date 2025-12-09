// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/AttributeGenerator.h"

FEcologyAttributes UAttributeGenerator::GenerateEcologyAttributes(int32 Seed, float ClimateValue, float TerrainValue)
{
    FRandomStream RandomStream = GetSeededRandomStream(Seed);
    FEcologyAttributes Result;

    // Generate habitat based on climate and terrain
    Result.Habitat = GenerateHabitatType(Seed, ClimateValue, TerrainValue);

    // Generate size class based on habitat
    Result.SizeClass = GenerateSizeClass(Seed + 1, Result.Habitat);

    // Generate tolerance (influenced by climate extremity)
    float ClimateExtremity = FMath::Abs(ClimateValue - 0.5f) * 2.0f; // 0 = moderate, 1 = extreme
    Result.Tolerance = FMath::Clamp(50.0f + (ClimateExtremity * 30.0f) + RandomStream.FRandRange(-10.0f, 10.0f), 0.0f, 100.0f);

    // Generate migration rate (influenced by terrain)
    Result.MigrationRate = FMath::Clamp(30.0f + (TerrainValue * 40.0f) + RandomStream.FRandRange(-15.0f, 15.0f), 0.0f, 100.0f);

    // Generate fecundity (random with slight habitat influence)
    float HabitatBonus = 0.0f;
    switch (Result.Habitat)
    {
    case EHabitatType::Forest:
    case EHabitatType::Swamp:
        HabitatBonus = 10.0f;
        break;
    case EHabitatType::Desert:
    case EHabitatType::Tundra:
        HabitatBonus = -10.0f;
        break;
    default:
        break;
    }
    Result.Fecundity = FMath::Clamp(50.0f + HabitatBonus + RandomStream.FRandRange(-20.0f, 20.0f), 0.0f, 100.0f);

    return Result;
}

FCombatAttributes UAttributeGenerator::GenerateCombatAttributes(int32 Seed, int32 NumTraits, ERarity MinRarity)
{
    FCombatAttributes Result;
    NumTraits = FMath::Clamp(NumTraits, 1, 3);

    for (int32 i = 0; i < NumTraits; i++)
    {
        FCombatTrait Trait = GenerateCombatTrait(Seed + i, MinRarity);
        Result.Traits.Add(Trait);
    }

    return Result;
}

FMonsterAttributes UAttributeGenerator::GenerateMonsterAttributes(int32 Seed, float ClimateValue, float TerrainValue, int32 NumTraits)
{
    FMonsterAttributes Result;

    // Generate ecology attributes
    Result.EcologyAttributes = GenerateEcologyAttributes(Seed, ClimateValue, TerrainValue);

    // Generate combat attributes
    Result.CombatAttributes = GenerateCombatAttributes(Seed + 1000, NumTraits);

    return Result;
}

EHabitatType UAttributeGenerator::GenerateHabitatType(int32 Seed, float ClimateValue, float TerrainValue)
{
    FRandomStream RandomStream = GetSeededRandomStream(Seed);

    // Map climate and terrain to habitat probabilities
    // ClimateValue: 0 = cold, 0.5 = moderate, 1 = hot
    // TerrainValue: 0 = flat, 0.5 = hilly, 1 = mountainous

    TArray<EHabitatType> PossibleHabitats;
    TArray<float> Weights;

    // Forest: moderate climate, moderate terrain
    PossibleHabitats.Add(EHabitatType::Forest);
    Weights.Add(FMath::Max(0.1f, 1.0f - FMath::Abs(ClimateValue - 0.5f) * 2.0f));

    // Desert: hot climate, flat terrain
    PossibleHabitats.Add(EHabitatType::Desert);
    Weights.Add(FMath::Max(0.1f, ClimateValue * (1.0f - TerrainValue)));

    // Tundra: cold climate, flat terrain
    PossibleHabitats.Add(EHabitatType::Tundra);
    Weights.Add(FMath::Max(0.1f, (1.0f - ClimateValue) * (1.0f - TerrainValue)));

    // Cave: any climate, mountainous terrain
    PossibleHabitats.Add(EHabitatType::Cave);
    Weights.Add(FMath::Max(0.1f, TerrainValue));

    // Volcano: hot climate, mountainous terrain
    PossibleHabitats.Add(EHabitatType::Volcano);
    Weights.Add(FMath::Max(0.1f, ClimateValue * TerrainValue));

    // Swamp: moderate climate, flat terrain
    PossibleHabitats.Add(EHabitatType::Swamp);
    Weights.Add(FMath::Max(0.1f, (1.0f - FMath::Abs(ClimateValue - 0.5f) * 2.0f) * (1.0f - TerrainValue)));

    // Select habitat based on weights
    float TotalWeight = 0.0f;
    for (float Weight : Weights)
    {
        TotalWeight += Weight;
    }

    float RandomValue = RandomStream.FRandRange(0.0f, TotalWeight);
    float AccumulatedWeight = 0.0f;
    for (int32 i = 0; i < PossibleHabitats.Num(); i++)
    {
        AccumulatedWeight += Weights[i];
        if (RandomValue <= AccumulatedWeight)
        {
            return PossibleHabitats[i];
        }
    }

    return EHabitatType::Forest; // Fallback
}

ESizeClass UAttributeGenerator::GenerateSizeClass(int32 Seed, EHabitatType Habitat)
{
    FRandomStream RandomStream = GetSeededRandomStream(Seed);

    // Different habitats have different size distributions
    TArray<ESizeClass> PossibleSizes;
    TArray<float> Weights;

    switch (Habitat)
    {
    case EHabitatType::Forest:
        // Forests support all sizes, with bias towards medium and large
        PossibleSizes = { ESizeClass::Tiny, ESizeClass::Small, ESizeClass::Medium, ESizeClass::Large, ESizeClass::Giant };
        Weights = { 0.1f, 0.2f, 0.3f, 0.3f, 0.1f };
        break;

    case EHabitatType::Desert:
        // Deserts favor small to medium sizes
        PossibleSizes = { ESizeClass::Tiny, ESizeClass::Small, ESizeClass::Medium, ESizeClass::Large };
        Weights = { 0.2f, 0.4f, 0.3f, 0.1f };
        break;

    case EHabitatType::Tundra:
        // Tundra favors medium to large sizes (cold adaptation)
        PossibleSizes = { ESizeClass::Small, ESizeClass::Medium, ESizeClass::Large, ESizeClass::Giant };
        Weights = { 0.1f, 0.3f, 0.4f, 0.2f };
        break;

    case EHabitatType::Cave:
        // Caves favor small to medium sizes (space constraints)
        PossibleSizes = { ESizeClass::Tiny, ESizeClass::Small, ESizeClass::Medium };
        Weights = { 0.3f, 0.5f, 0.2f };
        break;

    case EHabitatType::Volcano:
        // Volcanoes favor medium to giant sizes (extreme environment)
        PossibleSizes = { ESizeClass::Medium, ESizeClass::Large, ESizeClass::Giant };
        Weights = { 0.3f, 0.4f, 0.3f };
        break;

    case EHabitatType::Swamp:
        // Swamps support all sizes, with bias towards small and medium
        PossibleSizes = { ESizeClass::Tiny, ESizeClass::Small, ESizeClass::Medium, ESizeClass::Large };
        Weights = { 0.2f, 0.3f, 0.3f, 0.2f };
        break;

    default:
        PossibleSizes = { ESizeClass::Medium };
        Weights = { 1.0f };
        break;
    }

    // Select size based on weights
    float TotalWeight = 0.0f;
    for (float Weight : Weights)
    {
        TotalWeight += Weight;
    }

    float RandomValue = RandomStream.FRandRange(0.0f, TotalWeight);
    float AccumulatedWeight = 0.0f;
    for (int32 i = 0; i < PossibleSizes.Num(); i++)
    {
        AccumulatedWeight += Weights[i];
        if (RandomValue <= AccumulatedWeight)
        {
            return PossibleSizes[i];
        }
    }

    return ESizeClass::Medium; // Fallback
}

FCombatTrait UAttributeGenerator::GenerateCombatTrait(int32 Seed, ERarity MinRarity)
{
    FRandomStream RandomStream = GetSeededRandomStream(Seed);
    FCombatTrait Result;

    // TODO: Load trait library from data table instead of hardcoding
    // This is a temporary implementation for rapid prototyping.
    // In production, create a data table (DT_CombatTraits) with FCombatTraitData structure.
    // Benefits of using data table:
    // - Easy to add/modify traits without recompiling
    // - Designers can balance traits without touching code
    // - Support for localization
    // - Better organization and maintainability
    
    struct FTraitTemplate
    {
        FName ID;
        FString Name;
        FString Description;
        ERarity Rarity;
        TArray<FName> Parts;
    };

    TArray<FTraitTemplate> TraitLibrary = {
        { TEXT("BerserkLeap"), TEXT("狂暴跳跃"), TEXT("跳跃攻击，造成额外伤害"), ERarity::Common, { TEXT("StrongLegs") } },
        { TEXT("FireAffinity"), TEXT("火属性亲和"), TEXT("火焰攻击，有燃烧效果"), ERarity::Rare, { TEXT("FireWings"), TEXT("FireTail") } },
        { TEXT("StoneShell"), TEXT("金石外壳"), TEXT("高防御，减少受到的伤害"), ERarity::Rare, { TEXT("RockArmor") } },
        { TEXT("PoisonSting"), TEXT("剧毒之刺"), TEXT("攻击附带毒素效果"), ERarity::Common, { TEXT("PoisonStinger") } },
        { TEXT("IceBreath"), TEXT("冰霜吐息"), TEXT("冰冻敌人，降低移动速度"), ERarity::Epic, { TEXT("IceHorns"), TEXT("FrostAura") } },
        { TEXT("Regeneration"), TEXT("快速再生"), TEXT("持续恢复生命值"), ERarity::Epic, { TEXT("HealingGlow") } },
        { TEXT("SwiftStrike"), TEXT("疾风连击"), TEXT("快速连续攻击"), ERarity::Common, { TEXT("SharpClaws") } },
        { TEXT("ThunderRoar"), TEXT("雷鸣咆哮"), TEXT("范围震慑，造成眩晕"), ERarity::Rare, { TEXT("ThunderMane") } }
    };

    // Filter traits by minimum rarity
    TArray<FTraitTemplate> FilteredTraits;
    for (const FTraitTemplate& Trait : TraitLibrary)
    {
        if (static_cast<int32>(Trait.Rarity) >= static_cast<int32>(MinRarity))
        {
            FilteredTraits.Add(Trait);
        }
    }

    if (FilteredTraits.Num() == 0)
    {
        FilteredTraits = TraitLibrary; // Fallback to all traits
    }

    // Select a random trait
    int32 RandomIndex = RandomStream.RandRange(0, FilteredTraits.Num() - 1);
    const FTraitTemplate& SelectedTrait = FilteredTraits[RandomIndex];

    // Fill in the result
    Result.TraitID = SelectedTrait.ID;
    Result.TraitName = FText::FromString(SelectedTrait.Name);
    Result.TraitDescription = FText::FromString(SelectedTrait.Description);
    Result.Rarity = SelectedTrait.Rarity;
    Result.AssociatedPartIDs = SelectedTrait.Parts;

    // Generate effect parameters based on rarity
    float PowerMultiplier = 1.0f + static_cast<float>(static_cast<int32>(SelectedTrait.Rarity)) * 0.5f;
    Result.EffectParameters.Add(TEXT("DamageBonus"), PowerMultiplier);
    Result.EffectParameters.Add(TEXT("EffectChance"), 0.3f * PowerMultiplier);

    return Result;
}

void UAttributeGenerator::CalculateBaseStatsFromEcology(const FEcologyAttributes& EcologyAttributes, float& OutHP, float& OutATK, float& OutDEF, float& OutSPD)
{
    // Base stats
    float BaseHP = 100.0f;
    float BaseATK = 20.0f;
    float BaseDEF = 10.0f;
    float BaseSPD = 100.0f;

    // Size class modifiers
    float SizeHPMultiplier = 1.0f;
    float SizeATKMultiplier = 1.0f;
    float SizeDEFMultiplier = 1.0f;
    float SizeSPDMultiplier = 1.0f;

    switch (EcologyAttributes.SizeClass)
    {
    case ESizeClass::Tiny:
        SizeHPMultiplier = 0.5f;
        SizeATKMultiplier = 0.7f;
        SizeDEFMultiplier = 0.6f;
        SizeSPDMultiplier = 1.5f;
        break;
    case ESizeClass::Small:
        SizeHPMultiplier = 0.8f;
        SizeATKMultiplier = 0.9f;
        SizeDEFMultiplier = 0.8f;
        SizeSPDMultiplier = 1.2f;
        break;
    case ESizeClass::Medium:
        // Default values
        break;
    case ESizeClass::Large:
        SizeHPMultiplier = 1.4f;
        SizeATKMultiplier = 1.2f;
        SizeDEFMultiplier = 1.3f;
        SizeSPDMultiplier = 0.8f;
        break;
    case ESizeClass::Giant:
        SizeHPMultiplier = 2.0f;
        SizeATKMultiplier = 1.5f;
        SizeDEFMultiplier = 1.6f;
        SizeSPDMultiplier = 0.6f;
        break;
    }

    // Habitat modifiers
    float HabitatHPMultiplier = 1.0f;
    float HabitatATKMultiplier = 1.0f;
    float HabitatDEFMultiplier = 1.0f;
    float HabitatSPDMultiplier = 1.0f;

    switch (EcologyAttributes.Habitat)
    {
    case EHabitatType::Forest:
        HabitatHPMultiplier = 1.1f;
        HabitatSPDMultiplier = 1.1f;
        break;
    case EHabitatType::Desert:
        HabitatHPMultiplier = 1.2f;
        HabitatDEFMultiplier = 1.1f;
        break;
    case EHabitatType::Tundra:
        HabitatHPMultiplier = 0.9f;
        HabitatSPDMultiplier = 1.3f;
        break;
    case EHabitatType::Cave:
        HabitatDEFMultiplier = 1.2f;
        HabitatSPDMultiplier = 0.9f;
        break;
    case EHabitatType::Volcano:
        HabitatATKMultiplier = 1.3f;
        HabitatDEFMultiplier = 1.1f;
        break;
    case EHabitatType::Swamp:
        HabitatHPMultiplier = 1.1f;
        HabitatATKMultiplier = 1.1f;
        break;
    }

    // Calculate final stats
    OutHP = BaseHP * SizeHPMultiplier * HabitatHPMultiplier;
    OutATK = BaseATK * SizeATKMultiplier * HabitatATKMultiplier;
    OutDEF = BaseDEF * SizeDEFMultiplier * HabitatDEFMultiplier;
    OutSPD = BaseSPD * SizeSPDMultiplier * HabitatSPDMultiplier;
}

FRandomStream UAttributeGenerator::GetSeededRandomStream(int32 Seed)
{
    return FRandomStream(Seed);
}

template<typename TEnum>
TEnum UAttributeGenerator::GetRandomEnumValue(FRandomStream& RandomStream, int32 MaxValue)
{
    int32 RandomValue = RandomStream.RandRange(0, MaxValue - 1);
    return static_cast<TEnum>(RandomValue);
}
