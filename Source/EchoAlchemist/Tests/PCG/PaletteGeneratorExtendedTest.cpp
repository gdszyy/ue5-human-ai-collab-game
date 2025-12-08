// Copyright Echo Alchemist. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PCG/PaletteGeneratorExtended.h"
#include "PCG/MonsterAttributes.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// Test: Get Habitat Color Spectrum
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPaletteGeneratorExtendedTest_GetHabitatColorSpectrum,
    "EchoAlchemist.PCG.PaletteGeneratorExtended.GetHabitatColorSpectrum",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorExtendedTest_GetHabitatColorSpectrum::RunTest(const FString& Parameters)
{
    // Test all habitat types
    TArray<EHabitatType> Habitats = {
        EHabitatType::Forest,
        EHabitatType::Desert,
        EHabitatType::Tundra,
        EHabitatType::Cave,
        EHabitatType::Volcano,
        EHabitatType::Swamp
    };

    for (EHabitatType Habitat : Habitats)
    {
        FHabitatColorSpectrum Spectrum = UPaletteGeneratorExtended::GetHabitatColorSpectrum(Habitat);

        // Verify habitat is set correctly
        TestEqual(TEXT("Habitat type matches"), Spectrum.Habitat, Habitat);

        // Verify all spectrums have colors
        TestTrue(TEXT("Primary spectrum has colors"), Spectrum.PrimarySpectrum.Num() > 0);
        TestTrue(TEXT("Secondary spectrum has colors"), Spectrum.SecondarySpectrum.Num() > 0);
        TestTrue(TEXT("Accent spectrum has colors"), Spectrum.AccentSpectrum.Num() > 0);

        // Verify colors are valid (alpha = 1.0)
        for (const FLinearColor& Color : Spectrum.PrimarySpectrum)
        {
            TestEqual(TEXT("Primary color alpha is 1.0"), Color.A, 1.0f);
        }
        for (const FLinearColor& Color : Spectrum.SecondarySpectrum)
        {
            TestEqual(TEXT("Secondary color alpha is 1.0"), Color.A, 1.0f);
        }
        for (const FLinearColor& Color : Spectrum.AccentSpectrum)
        {
            TestEqual(TEXT("Accent color alpha is 1.0"), Color.A, 1.0f);
        }
    }

    return true;
}

// ============================================================================
// Test: Generate Monster Palette
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPaletteGeneratorExtendedTest_GenerateMonsterPalette,
    "EchoAlchemist.PCG.PaletteGeneratorExtended.GenerateMonsterPalette",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorExtendedTest_GenerateMonsterPalette::RunTest(const FString& Parameters)
{
    // Create test ecology attributes
    FEcologyAttributes EcologyAttrs;
    EcologyAttrs.Habitat = EHabitatType::Forest;
    EcologyAttrs.SizeClass = ESizeClass::Medium;
    EcologyAttrs.Tolerance = 50.0f;

    // Create test combat attributes
    FCombatAttributes CombatAttrs;
    FCombatTrait FireTrait;
    FireTrait.TraitID = FName(TEXT("FireAffinity"));
    FireTrait.Rarity = ERarity::Rare;
    CombatAttrs.Traits.Add(FireTrait);

    // Generate palette
    FMonsterColorPalette Palette = UPaletteGeneratorExtended::GenerateMonsterPalette(
        EcologyAttrs, CombatAttrs, 12345);

    // Verify palette has valid colors
    TestTrue(TEXT("Primary color is valid"), Palette.PrimaryColor.A == 1.0f);
    TestTrue(TEXT("Secondary color is valid"), Palette.SecondaryColor.A == 1.0f);
    TestTrue(TEXT("Accent color is valid"), Palette.AccentColor.A == 1.0f);

    // Verify colors are different
    TestNotEqual(TEXT("Primary and secondary colors are different"),
        Palette.PrimaryColor, Palette.SecondaryColor);
    TestNotEqual(TEXT("Primary and accent colors are different"),
        Palette.PrimaryColor, Palette.AccentColor);

    // Test determinism: same seed should produce same palette
    FMonsterColorPalette Palette2 = UPaletteGeneratorExtended::GenerateMonsterPalette(
        EcologyAttrs, CombatAttrs, 12345);

    TestEqual(TEXT("Same seed produces same primary color"),
        Palette.PrimaryColor, Palette2.PrimaryColor);
    TestEqual(TEXT("Same seed produces same secondary color"),
        Palette.SecondaryColor, Palette2.SecondaryColor);
    TestEqual(TEXT("Same seed produces same accent color"),
        Palette.AccentColor, Palette2.AccentColor);

    // Test different seed produces different palette
    FMonsterColorPalette Palette3 = UPaletteGeneratorExtended::GenerateMonsterPalette(
        EcologyAttrs, CombatAttrs, 54321);

    TestNotEqual(TEXT("Different seed produces different primary color"),
        Palette.PrimaryColor, Palette3.PrimaryColor);

    return true;
}

// ============================================================================
// Test: Adjust Palette By Rarity
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPaletteGeneratorExtendedTest_AdjustPaletteByRarity,
    "EchoAlchemist.PCG.PaletteGeneratorExtended.AdjustPaletteByRarity",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorExtendedTest_AdjustPaletteByRarity::RunTest(const FString& Parameters)
{
    // Create base palette
    FMonsterColorPalette BasePalette;
    BasePalette.PrimaryColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
    BasePalette.SecondaryColor = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);
    BasePalette.AccentColor = FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);

    // Test common rarity (no change)
    FMonsterColorPalette CommonPalette = UPaletteGeneratorExtended::AdjustPaletteByRarity(
        BasePalette, ERarity::Common);

    TestEqual(TEXT("Common rarity does not change primary color"),
        BasePalette.PrimaryColor, CommonPalette.PrimaryColor);

    // Test rare rarity (more saturated and brighter)
    FMonsterColorPalette RarePalette = UPaletteGeneratorExtended::AdjustPaletteByRarity(
        BasePalette, ERarity::Rare);

    // Convert to HSV to check saturation and value
    FLinearColor BaseHSV = BasePalette.PrimaryColor.LinearRGBToHSV();
    FLinearColor RareHSV = RarePalette.PrimaryColor.LinearRGBToHSV();

    TestTrue(TEXT("Rare rarity increases saturation or value"),
        RareHSV.G >= BaseHSV.G || RareHSV.B >= BaseHSV.B);

    // Test epic rarity (highly saturated and bright)
    FMonsterColorPalette EpicPalette = UPaletteGeneratorExtended::AdjustPaletteByRarity(
        BasePalette, ERarity::Epic);

    FLinearColor EpicHSV = EpicPalette.PrimaryColor.LinearRGBToHSV();

    TestTrue(TEXT("Epic rarity increases saturation or value more than rare"),
        EpicHSV.G >= RareHSV.G || EpicHSV.B >= RareHSV.B);

    return true;
}

// ============================================================================
// Test: Adjust Palette By Traits
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPaletteGeneratorExtendedTest_AdjustPaletteByTraits,
    "EchoAlchemist.PCG.PaletteGeneratorExtended.AdjustPaletteByTraits",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorExtendedTest_AdjustPaletteByTraits::RunTest(const FString& Parameters)
{
    // Create base palette
    FMonsterColorPalette BasePalette;
    BasePalette.PrimaryColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
    BasePalette.SecondaryColor = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);
    BasePalette.AccentColor = FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);

    // Create fire trait
    TArray<FCombatTrait> FireTraits;
    FCombatTrait FireTrait;
    FireTrait.TraitID = FName(TEXT("FireAffinity"));
    FireTraits.Add(FireTrait);

    // Adjust palette by fire trait
    FMonsterColorPalette FirePalette = UPaletteGeneratorExtended::AdjustPaletteByTraits(
        BasePalette, FireTraits);

    // Verify that accent color has changed (fire influence)
    TestNotEqual(TEXT("Fire trait changes accent color"),
        BasePalette.AccentColor, FirePalette.AccentColor);

    // Create ice trait
    TArray<FCombatTrait> IceTraits;
    FCombatTrait IceTrait;
    IceTrait.TraitID = FName(TEXT("IceAffinity"));
    IceTraits.Add(IceTrait);

    // Adjust palette by ice trait
    FMonsterColorPalette IcePalette = UPaletteGeneratorExtended::AdjustPaletteByTraits(
        BasePalette, IceTraits);

    // Verify that ice and fire produce different results
    TestNotEqual(TEXT("Fire and ice traits produce different accent colors"),
        FirePalette.AccentColor, IcePalette.AccentColor);

    return true;
}

// ============================================================================
// Test: Expand Monster Palette
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPaletteGeneratorExtendedTest_ExpandMonsterPalette,
    "EchoAlchemist.PCG.PaletteGeneratorExtended.ExpandMonsterPalette",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorExtendedTest_ExpandMonsterPalette::RunTest(const FString& Parameters)
{
    // Create monster palette
    FMonsterColorPalette MonsterPalette;
    MonsterPalette.PrimaryColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);   // Red
    MonsterPalette.SecondaryColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green
    MonsterPalette.AccentColor = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);    // Blue

    // Expand to 8 colors
    FPalette ExpandedPalette = UPaletteGeneratorExtended::ExpandMonsterPalette(
        MonsterPalette, 8);

    // Verify correct number of colors
    TestEqual(TEXT("Expanded palette has correct number of colors"),
        ExpandedPalette.Colors.Num(), 8);

    // Verify first and last colors are close to original colors
    TestTrue(TEXT("First color is close to primary"),
        ExpandedPalette.Colors[0].Equals(MonsterPalette.PrimaryColor, 0.1f));

    // Test edge case: 0 colors
    FPalette EmptyPalette = UPaletteGeneratorExtended::ExpandMonsterPalette(
        MonsterPalette, 0);

    TestEqual(TEXT("Empty palette has 0 colors"), EmptyPalette.Colors.Num(), 0);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
