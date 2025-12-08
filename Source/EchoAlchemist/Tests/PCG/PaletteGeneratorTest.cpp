// Copyright Echo Alchemist. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PCG/PaletteGenerator.h"
#include "PCG/MonsterAttributes.h"

#if WITH_DEV_AUTOMATION_TESTS

// ========================================
// Habitat Color Spectrum Tests
// ========================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorHabitatSpectrumTest, "EchoAlchemist.PCG.PaletteGenerator.HabitatSpectrum", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorHabitatSpectrumTest::RunTest(const FString& Parameters)
{
    // Test Forest habitat spectrum
    FHabitatColorSpectrum ForestSpectrum = UPaletteGenerator::GetHabitatColorSpectrum(EHabitatType::Forest);
    TestTrue(TEXT("Forest hue range should be in green range (90-150)"), 
        ForestSpectrum.HueRange.X >= 90.0f && ForestSpectrum.HueRange.Y <= 150.0f);

    // Test Desert habitat spectrum
    FHabitatColorSpectrum DesertSpectrum = UPaletteGenerator::GetHabitatColorSpectrum(EHabitatType::Desert);
    TestTrue(TEXT("Desert hue range should be in yellow-orange range (30-60)"), 
        DesertSpectrum.HueRange.X >= 30.0f && DesertSpectrum.HueRange.Y <= 60.0f);

    // Test Tundra habitat spectrum
    FHabitatColorSpectrum TundraSpectrum = UPaletteGenerator::GetHabitatColorSpectrum(EHabitatType::Tundra);
    TestTrue(TEXT("Tundra hue range should be in blue range (180-240)"), 
        TundraSpectrum.HueRange.X >= 180.0f && TundraSpectrum.HueRange.Y <= 240.0f);
    TestTrue(TEXT("Tundra should have high brightness"), TundraSpectrum.ValueRange.Y >= 0.9f);

    // Test Cave habitat spectrum
    FHabitatColorSpectrum CaveSpectrum = UPaletteGenerator::GetHabitatColorSpectrum(EHabitatType::Cave);
    TestTrue(TEXT("Cave should have low brightness"), CaveSpectrum.ValueRange.Y <= 0.5f);

    // Test Volcano habitat spectrum
    FHabitatColorSpectrum VolcanoSpectrum = UPaletteGenerator::GetHabitatColorSpectrum(EHabitatType::Volcano);
    TestTrue(TEXT("Volcano hue range should be in red-orange range (0-30)"), 
        VolcanoSpectrum.HueRange.X >= 0.0f && VolcanoSpectrum.HueRange.Y <= 30.0f);
    TestTrue(TEXT("Volcano should have high saturation"), VolcanoSpectrum.SaturationRange.X >= 0.7f);

    // Test Swamp habitat spectrum
    FHabitatColorSpectrum SwampSpectrum = UPaletteGenerator::GetHabitatColorSpectrum(EHabitatType::Swamp);
    TestTrue(TEXT("Swamp should have low brightness"), SwampSpectrum.ValueRange.Y <= 0.5f);

    return true;
}

// ========================================
// Ecology-Based Palette Generation Tests
// ========================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorEcologyTest, "EchoAlchemist.PCG.PaletteGenerator.EcologyPalette", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorEcologyTest::RunTest(const FString& Parameters)
{
    // Create test ecology attributes
    FEcologyAttributes ForestEcology;
    ForestEcology.Habitat = EHabitatType::Forest;
    ForestEcology.SizeClass = ESizeClass::Medium;
    ForestEcology.Tolerance = 50.0f;

    // Generate palette
    FPalette Palette = UPaletteGenerator::GeneratePaletteFromEcology(ForestEcology, 12345);

    // Verify palette has all required colors
    TestTrue(TEXT("Palette should have primary color"), !Palette.PrimaryColor.Equals(FLinearColor::Black));
    TestTrue(TEXT("Palette should have secondary color"), !Palette.SecondaryColor.Equals(FLinearColor::Black));
    TestTrue(TEXT("Palette should have highlight color"), !Palette.HighlightColor.Equals(FLinearColor::Black));
    TestTrue(TEXT("Palette should have shadow color"), !Palette.ShadowColor.Equals(FLinearColor::Black));
    TestTrue(TEXT("Palette Colors array should have 4 colors"), Palette.Colors.Num() == 4);

    // Test determinism (same seed should produce same result)
    FPalette Palette2 = UPaletteGenerator::GeneratePaletteFromEcology(ForestEcology, 12345);
    TestTrue(TEXT("Same seed should produce same primary color"), 
        Palette.PrimaryColor.Equals(Palette2.PrimaryColor, 0.001f));

    // Test different seeds produce different results
    FPalette Palette3 = UPaletteGenerator::GeneratePaletteFromEcology(ForestEcology, 67890);
    TestFalse(TEXT("Different seeds should produce different primary colors"), 
        Palette.PrimaryColor.Equals(Palette3.PrimaryColor, 0.001f));

    return true;
}

// ========================================
// Size Class Influence Tests
// ========================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorSizeClassTest, "EchoAlchemist.PCG.PaletteGenerator.SizeClassInfluence", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorSizeClassTest::RunTest(const FString& Parameters)
{
    // Create ecology attributes with different size classes
    FEcologyAttributes TinyEcology;
    TinyEcology.Habitat = EHabitatType::Forest;
    TinyEcology.SizeClass = ESizeClass::Tiny;
    TinyEcology.Tolerance = 50.0f;

    FEcologyAttributes GiantEcology;
    GiantEcology.Habitat = EHabitatType::Forest;
    GiantEcology.SizeClass = ESizeClass::Giant;
    GiantEcology.Tolerance = 50.0f;

    // Generate palettes
    FPalette TinyPalette = UPaletteGenerator::GeneratePaletteFromEcology(TinyEcology, 12345);
    FPalette GiantPalette = UPaletteGenerator::GeneratePaletteFromEcology(GiantEcology, 12345);

    // Convert to HSV for comparison
    FLinearColor TinyHSV = TinyPalette.PrimaryColor.LinearRGBToHSV();
    FLinearColor GiantHSV = GiantPalette.PrimaryColor.LinearRGBToHSV();

    // Giant creatures should have darker colors (lower V value)
    TestTrue(TEXT("Giant creatures should have darker colors than tiny creatures"), 
        GiantHSV.B < TinyHSV.B);

    return true;
}

// ========================================
// Rarity Adjustment Tests
// ========================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorRarityTest, "EchoAlchemist.PCG.PaletteGenerator.RarityAdjustment", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorRarityTest::RunTest(const FString& Parameters)
{
    // Create a base color
    FLinearColor BaseColor = FLinearColor(0.5f, 0.7f, 0.3f); // Green color

    // Adjust by different rarities
    FLinearColor CommonColor = UPaletteGenerator::AdjustColorByRarity(BaseColor, ERarity::Common);
    FLinearColor RareColor = UPaletteGenerator::AdjustColorByRarity(BaseColor, ERarity::Rare);
    FLinearColor EpicColor = UPaletteGenerator::AdjustColorByRarity(BaseColor, ERarity::Epic);

    // Convert to HSV for comparison
    FLinearColor CommonHSV = CommonColor.LinearRGBToHSV();
    FLinearColor RareHSV = RareColor.LinearRGBToHSV();
    FLinearColor EpicHSV = EpicColor.LinearRGBToHSV();

    // Common should be unchanged
    TestTrue(TEXT("Common rarity should not change color"), 
        CommonColor.Equals(BaseColor, 0.001f));

    // Rare should have higher saturation
    TestTrue(TEXT("Rare rarity should increase saturation"), 
        RareHSV.G > CommonHSV.G);

    // Epic should have highest saturation and brightness
    TestTrue(TEXT("Epic rarity should have highest saturation"), 
        EpicHSV.G > RareHSV.G);
    TestTrue(TEXT("Epic rarity should increase brightness"), 
        EpicHSV.B > CommonHSV.B);

    return true;
}

// ========================================
// Combat Trait Accent Color Tests
// ========================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorTraitAccentTest, "EchoAlchemist.PCG.PaletteGenerator.TraitAccentColor", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorTraitAccentTest::RunTest(const FString& Parameters)
{
    // Test fire trait accent color
    FLinearColor FireAccent = UPaletteGenerator::GetTraitAccentColor(FName(TEXT("FireAffinity")));
    TestTrue(TEXT("Fire trait should have red-orange accent"), 
        FireAccent.R > 0.8f && FireAccent.G < 0.5f);

    // Test ice trait accent color
    FLinearColor IceAccent = UPaletteGenerator::GetTraitAccentColor(FName(TEXT("IceAffinity")));
    TestTrue(TEXT("Ice trait should have cyan-blue accent"), 
        IceAccent.B > 0.8f && IceAccent.R < 0.3f);

    // Test poison trait accent color
    FLinearColor PoisonAccent = UPaletteGenerator::GetTraitAccentColor(FName(TEXT("PoisonAffinity")));
    TestTrue(TEXT("Poison trait should have green accent"), 
        PoisonAccent.G > 0.8f);

    // Test unknown trait (should return white)
    FLinearColor UnknownAccent = UPaletteGenerator::GetTraitAccentColor(FName(TEXT("UnknownTrait")));
    TestTrue(TEXT("Unknown trait should return white"), 
        UnknownAccent.Equals(FLinearColor::White, 0.001f));

    return true;
}

// ========================================
// Full Monster Attributes Palette Generation Tests
// ========================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorMonsterAttributesTest, "EchoAlchemist.PCG.PaletteGenerator.MonsterAttributesPalette", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorMonsterAttributesTest::RunTest(const FString& Parameters)
{
    // Create test monster attributes
    FMonsterAttributes MonsterAttrs;
    
    // Ecology: Forest, Medium size
    MonsterAttrs.EcologyAttributes.Habitat = EHabitatType::Forest;
    MonsterAttrs.EcologyAttributes.SizeClass = ESizeClass::Medium;
    MonsterAttrs.EcologyAttributes.Tolerance = 50.0f;

    // Combat: Epic rarity, Fire affinity trait
    FCombatTrait FireTrait;
    FireTrait.TraitID = FName(TEXT("FireAffinity"));
    FireTrait.TraitName = FText::FromString(TEXT("Fire Affinity"));
    FireTrait.Rarity = ERarity::Epic;
    MonsterAttrs.CombatAttributes.Traits.Add(FireTrait);

    // Generate palette
    FPalette Palette = UPaletteGenerator::GeneratePaletteFromMonsterAttributes(MonsterAttrs, 12345);

    // Verify palette has all required colors
    TestTrue(TEXT("Palette should have primary color"), !Palette.PrimaryColor.Equals(FLinearColor::Black));
    TestTrue(TEXT("Palette should have secondary color"), !Palette.SecondaryColor.Equals(FLinearColor::Black));
    TestTrue(TEXT("Palette should have highlight color"), !Palette.HighlightColor.Equals(FLinearColor::Black));
    TestTrue(TEXT("Palette should have shadow color"), !Palette.ShadowColor.Equals(FLinearColor::Black));

    // Verify highlight color has fire accent influence
    FLinearColor HighlightHSV = Palette.HighlightColor.LinearRGBToHSV();
    TestTrue(TEXT("Highlight color should be vibrant (high saturation) due to Epic rarity"), 
        HighlightHSV.G > 0.6f);

    return true;
}

// ========================================
// Combat Attribute Modification Tests
// ========================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorCombatModificationTest, "EchoAlchemist.PCG.PaletteGenerator.CombatModification", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorCombatModificationTest::RunTest(const FString& Parameters)
{
    // Create a base palette
    FPalette BasePalette;
    BasePalette.PrimaryColor = FLinearColor(0.3f, 0.6f, 0.2f); // Green
    BasePalette.SecondaryColor = FLinearColor(0.2f, 0.5f, 0.15f);
    BasePalette.HighlightColor = FLinearColor(0.5f, 0.8f, 0.4f);
    BasePalette.ShadowColor = FLinearColor(0.1f, 0.3f, 0.05f);

    // Create combat attributes with fire trait
    FCombatAttributes CombatAttrs;
    FCombatTrait FireTrait;
    FireTrait.TraitID = FName(TEXT("FireAffinity"));
    FireTrait.Rarity = ERarity::Rare;
    CombatAttrs.Traits.Add(FireTrait);

    // Modify palette
    FPalette ModifiedPalette = UPaletteGenerator::ModifyPaletteWithCombatAttributes(BasePalette, CombatAttrs, 12345);

    // Verify colors were modified
    TestFalse(TEXT("Primary color should be modified by rarity"), 
        ModifiedPalette.PrimaryColor.Equals(BasePalette.PrimaryColor, 0.001f));
    TestFalse(TEXT("Highlight color should be modified by trait accent"), 
        ModifiedPalette.HighlightColor.Equals(BasePalette.HighlightColor, 0.001f));

    // Test with no traits
    FCombatAttributes EmptyCombatAttrs;
    FPalette UnmodifiedPalette = UPaletteGenerator::ModifyPaletteWithCombatAttributes(BasePalette, EmptyCombatAttrs, 12345);
    TestTrue(TEXT("Palette with no traits should only adjust by common rarity"), 
        !UnmodifiedPalette.PrimaryColor.Equals(FLinearColor::Black));

    return true;
}

// ========================================
// Legacy Function Tests
// ========================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorLegacyTest, "EchoAlchemist.PCG.PaletteGenerator.LegacyFunctions", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorLegacyTest::RunTest(const FString& Parameters)
{
    // Test GeneratePaletteFromSpectrum
    TArray<FLinearColor> Spectrum;
    Spectrum.Add(FLinearColor::Red);
    Spectrum.Add(FLinearColor::Yellow);
    Spectrum.Add(FLinearColor::Green);

    FPalette SpectrumPalette = UPaletteGenerator::GeneratePaletteFromSpectrum(Spectrum, 5);
    TestTrue(TEXT("Spectrum palette should have 5 colors"), SpectrumPalette.Colors.Num() == 5);

    // Test GenerateMonochromaticPalette
    FPalette MonoPalette = UPaletteGenerator::GenerateMonochromaticPalette(FLinearColor::Blue, 4);
    TestTrue(TEXT("Monochromatic palette should have 4 colors"), MonoPalette.Colors.Num() == 4);

    // Verify all colors in monochromatic palette have similar hue
    FLinearColor FirstHSV = MonoPalette.Colors[0].LinearRGBToHSV();
    FLinearColor LastHSV = MonoPalette.Colors[MonoPalette.Colors.Num() - 1].LinearRGBToHSV();
    TestTrue(TEXT("Monochromatic palette should have consistent hue"), 
        FMath::Abs(FirstHSV.R - LastHSV.R) < 10.0f); // Allow 10 degree variation

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
