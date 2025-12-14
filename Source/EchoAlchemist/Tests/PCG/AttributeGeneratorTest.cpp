// Copyright Echo Alchemist. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PCG/AttributeGenerator.h"
#include "PCG/MonsterAttributes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAttributeGeneratorEcologyTest, "EchoAlchemist.PCG.AttributeGenerator.GenerateEcologyAttributes", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAttributeGeneratorEcologyTest::RunTest(const FString& Parameters)
{
    // Test: Generate ecology attributes with deterministic seed
    int32 Seed = 12345;
    float ClimateValue = 0.7f; // Hot climate
    float TerrainValue = 0.3f; // Flat terrain

    FEcologyAttributes Ecology = UAttributeGenerator::GenerateEcologyAttributes(Seed, ClimateValue, TerrainValue);

    // Verify that attributes are within valid ranges
    TestTrue(TEXT("Tolerance should be between 0 and 100"), Ecology.Tolerance >= 0.0f && Ecology.Tolerance <= 100.0f);
    TestTrue(TEXT("Migration rate should be between 0 and 100"), Ecology.MigrationRate >= 0.0f && Ecology.MigrationRate <= 100.0f);
    TestTrue(TEXT("Fecundity should be between 0 and 100"), Ecology.Fecundity >= 0.0f && Ecology.Fecundity <= 100.0f);

    // Test determinism: same seed should produce same result
    FEcologyAttributes Ecology2 = UAttributeGenerator::GenerateEcologyAttributes(Seed, ClimateValue, TerrainValue);
    TestEqual(TEXT("Same seed should produce same habitat"), Ecology.Habitat, Ecology2.Habitat);
    TestEqual(TEXT("Same seed should produce same size class"), Ecology.SizeClass, Ecology2.SizeClass);
    TestEqual(TEXT("Same seed should produce same tolerance"), Ecology.Tolerance, Ecology2.Tolerance);

    // Test: Different seed should produce different result
    FEcologyAttributes Ecology3 = UAttributeGenerator::GenerateEcologyAttributes(Seed + 1, ClimateValue, TerrainValue);
    bool bDifferent = (Ecology.Habitat != Ecology3.Habitat) || 
                      (Ecology.SizeClass != Ecology3.SizeClass) ||
                      (FMath::Abs(Ecology.Tolerance - Ecology3.Tolerance) > 0.01f);
    TestTrue(TEXT("Different seed should likely produce different attributes"), bDifferent);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAttributeGeneratorCombatTest, "EchoAlchemist.PCG.AttributeGenerator.GenerateCombatAttributes", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAttributeGeneratorCombatTest::RunTest(const FString& Parameters)
{
    // Test: Generate combat attributes with 2 traits
    int32 Seed = 67890;
    int32 NumTraits = 2;
    ERarity MinRarity = ERarity::Common;

    FCombatAttributes Combat = UAttributeGenerator::GenerateCombatAttributes(Seed, NumTraits, MinRarity);

    // Verify trait count
    TestEqual(TEXT("Should generate 2 traits"), Combat.Traits.Num(), 2);

    // Verify each trait has valid data
    for (const FCombatTrait& Trait : Combat.Traits)
    {
        TestTrue(TEXT("Trait ID should not be empty"), Trait.TraitID != NAME_None);
        TestTrue(TEXT("Trait name should not be empty"), !Trait.TraitName.IsEmpty());
        TestTrue(TEXT("Trait should have effect parameters"), Trait.EffectParameters.Num() > 0);
    }

    // Test: Generate with minimum rarity filter
    FCombatAttributes CombatRare = UAttributeGenerator::GenerateCombatAttributes(Seed, 3, ERarity::Rare);
    TestEqual(TEXT("Should generate 3 traits"), CombatRare.Traits.Num(), 3);
    
    for (const FCombatTrait& Trait : CombatRare.Traits)
    {
        TestTrue(TEXT("Trait rarity should be at least Rare"), 
                 static_cast<int32>(Trait.Rarity) >= static_cast<int32>(ERarity::Rare));
    }

    // Test determinism
    FCombatAttributes Combat2 = UAttributeGenerator::GenerateCombatAttributes(Seed, NumTraits, MinRarity);
    TestEqual(TEXT("Same seed should produce same number of traits"), Combat.Traits.Num(), Combat2.Traits.Num());
    if (Combat.Traits.Num() > 0 && Combat2.Traits.Num() > 0)
    {
        TestEqual(TEXT("Same seed should produce same first trait ID"), Combat.Traits[0].TraitID, Combat2.Traits[0].TraitID);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAttributeGeneratorMonsterTest, "EchoAlchemist.PCG.AttributeGenerator.GenerateMonsterAttributes", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAttributeGeneratorMonsterTest::RunTest(const FString& Parameters)
{
    // Test: Generate complete monster attributes
    int32 Seed = 11111;
    float ClimateValue = 0.3f; // Cold climate
    float TerrainValue = 0.8f; // Mountainous terrain
    int32 NumTraits = 2;

    FMonsterAttributes Monster = UAttributeGenerator::GenerateMonsterAttributes(Seed, ClimateValue, TerrainValue, NumTraits);

    // Verify ecology attributes
    TestTrue(TEXT("Ecology tolerance should be valid"), 
             Monster.EcologyAttributes.Tolerance >= 0.0f && Monster.EcologyAttributes.Tolerance <= 100.0f);
    TestTrue(TEXT("Ecology migration rate should be valid"), 
             Monster.EcologyAttributes.MigrationRate >= 0.0f && Monster.EcologyAttributes.MigrationRate <= 100.0f);
    TestTrue(TEXT("Ecology fecundity should be valid"), 
             Monster.EcologyAttributes.Fecundity >= 0.0f && Monster.EcologyAttributes.Fecundity <= 100.0f);

    // Verify combat attributes
    TestEqual(TEXT("Should have specified number of combat traits"), Monster.CombatAttributes.Traits.Num(), NumTraits);

    // Test determinism
    FMonsterAttributes Monster2 = UAttributeGenerator::GenerateMonsterAttributes(Seed, ClimateValue, TerrainValue, NumTraits);
    TestEqual(TEXT("Same seed should produce same habitat"), 
              Monster.EcologyAttributes.Habitat, Monster2.EcologyAttributes.Habitat);
    TestEqual(TEXT("Same seed should produce same size class"), 
              Monster.EcologyAttributes.SizeClass, Monster2.EcologyAttributes.SizeClass);
    TestEqual(TEXT("Same seed should produce same number of traits"), 
              Monster.CombatAttributes.Traits.Num(), Monster2.CombatAttributes.Traits.Num());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAttributeGeneratorHabitatTest, "EchoAlchemist.PCG.AttributeGenerator.GenerateHabitatType", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAttributeGeneratorHabitatTest::RunTest(const FString& Parameters)
{
    // Test: Hot climate + flat terrain should favor Desert
    int32 Seed = 12345;
    EHabitatType Habitat1 = UAttributeGenerator::GenerateHabitatType(Seed, 0.9f, 0.1f);
    // Note: Due to randomness, we can't guarantee Desert, but we can test determinism

    // Test determinism
    EHabitatType Habitat2 = UAttributeGenerator::GenerateHabitatType(Seed, 0.9f, 0.1f);
    TestEqual(TEXT("Same seed and world state should produce same habitat"), Habitat1, Habitat2);

    // Test: Cold climate + flat terrain should favor Tundra
    EHabitatType Habitat3 = UAttributeGenerator::GenerateHabitatType(Seed, 0.1f, 0.1f);
    EHabitatType Habitat4 = UAttributeGenerator::GenerateHabitatType(Seed, 0.1f, 0.1f);
    TestEqual(TEXT("Same seed and world state should produce same habitat (cold)"), Habitat3, Habitat4);

    // Test: Moderate climate + moderate terrain should favor Forest
    EHabitatType Habitat5 = UAttributeGenerator::GenerateHabitatType(Seed, 0.5f, 0.5f);
    EHabitatType Habitat6 = UAttributeGenerator::GenerateHabitatType(Seed, 0.5f, 0.5f);
    TestEqual(TEXT("Same seed and world state should produce same habitat (moderate)"), Habitat5, Habitat6);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAttributeGeneratorSizeClassTest, "EchoAlchemist.PCG.AttributeGenerator.GenerateSizeClass", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAttributeGeneratorSizeClassTest::RunTest(const FString& Parameters)
{
    // Test: Generate size class for different habitats
    int32 Seed = 67890;

    // Test Forest habitat (supports all sizes)
    ESizeClass ForestSize = UAttributeGenerator::GenerateSizeClass(Seed, EHabitatType::Forest);
    TestTrue(TEXT("Forest size should be valid"), 
             ForestSize >= ESizeClass::Tiny && ForestSize <= ESizeClass::Giant);

    // Test Cave habitat (favors small sizes)
    ESizeClass CaveSize = UAttributeGenerator::GenerateSizeClass(Seed, EHabitatType::Cave);
    TestTrue(TEXT("Cave size should be small to medium"), 
             CaveSize >= ESizeClass::Tiny && CaveSize <= ESizeClass::Medium);

    // Test determinism
    ESizeClass ForestSize2 = UAttributeGenerator::GenerateSizeClass(Seed, EHabitatType::Forest);
    TestEqual(TEXT("Same seed and habitat should produce same size"), ForestSize, ForestSize2);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAttributeGeneratorStatsTest, "EchoAlchemist.PCG.AttributeGenerator.CalculateBaseStatsFromEcology", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAttributeGeneratorStatsTest::RunTest(const FString& Parameters)
{
    // Test: Calculate stats for a medium-sized forest monster
    FEcologyAttributes Ecology;
    Ecology.Habitat = EHabitatType::Forest;
    Ecology.SizeClass = ESizeClass::Medium;
    Ecology.Tolerance = 50.0f;
    Ecology.MigrationRate = 30.0f;
    Ecology.Fecundity = 50.0f;

    float HP, ATK, DEF, SPD;
    UAttributeGenerator::CalculateBaseStatsFromEcology(Ecology, HP, ATK, DEF, SPD);

    // Verify stats are positive
    TestTrue(TEXT("HP should be positive"), HP > 0.0f);
    TestTrue(TEXT("ATK should be positive"), ATK > 0.0f);
    TestTrue(TEXT("DEF should be positive"), DEF > 0.0f);
    TestTrue(TEXT("SPD should be positive"), SPD > 0.0f);

    // Test: Larger size should have higher HP
    FEcologyAttributes LargeEcology = Ecology;
    LargeEcology.SizeClass = ESizeClass::Giant;
    float LargeHP, LargeATK, LargeDEF, LargeSPD;
    UAttributeGenerator::CalculateBaseStatsFromEcology(LargeEcology, LargeHP, LargeATK, LargeDEF, LargeSPD);

    TestTrue(TEXT("Giant size should have more HP than Medium"), LargeHP > HP);
    TestTrue(TEXT("Giant size should have more ATK than Medium"), LargeATK > ATK);
    TestTrue(TEXT("Giant size should be slower than Medium"), LargeSPD < SPD);

    // Test: Smaller size should have higher SPD
    FEcologyAttributes TinyEcology = Ecology;
    TinyEcology.SizeClass = ESizeClass::Tiny;
    float TinyHP, TinyATK, TinyDEF, TinySPD;
    UAttributeGenerator::CalculateBaseStatsFromEcology(TinyEcology, TinyHP, TinyATK, TinyDEF, TinySPD);

    TestTrue(TEXT("Tiny size should have less HP than Medium"), TinyHP < HP);
    TestTrue(TEXT("Tiny size should be faster than Medium"), TinySPD > SPD);

    return true;
}
