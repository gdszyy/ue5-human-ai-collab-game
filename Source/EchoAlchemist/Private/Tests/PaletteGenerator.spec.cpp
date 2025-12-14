#include "CoreTypes.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"
#include "PCG/PaletteGenerator.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorTest, "Project.UnitTests.PCG.PaletteGenerator", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

bool FPaletteGeneratorTest::RunTest(const FString& Parameters)
{
    // Test 1: GeneratePaletteFromSpectrum
    TArray<FLinearColor> Spectrum;
    Spectrum.Add(FLinearColor::Red);
    Spectrum.Add(FLinearColor::Blue);

    FPalette Palette = UPaletteGenerator::GeneratePaletteFromSpectrum(Spectrum, 3);
    TestEqual("Spectrum Palette should have 3 colors", Palette.Colors.Num(), 3);
    TestTrue("First color should be Red", Palette.Colors[0].Equals(FLinearColor::Red, 0.01f));
    TestTrue("Last color should be Blue", Palette.Colors[2].Equals(FLinearColor::Blue, 0.01f));

    // Test 2: GenerateMonochromaticPalette
    FPalette MonoPalette = UPaletteGenerator::GenerateMonochromaticPalette(FLinearColor::Green, 5);
    TestEqual("Monochromatic Palette should have 5 colors", MonoPalette.Colors.Num(), 5);

    // Test 3: Edge case - empty spectrum
    FPalette EmptyPalette = UPaletteGenerator::GeneratePaletteFromSpectrum(TArray<FLinearColor>(), 5);
    TestEqual("Empty spectrum should result in an empty palette", EmptyPalette.Colors.Num(), 0);

    // Test 4: Edge case - zero colors
    FPalette ZeroColorPalette = UPaletteGenerator::GeneratePaletteFromSpectrum(Spectrum, 0);
    TestEqual("Zero colors should result in an empty palette", ZeroColorPalette.Colors.Num(), 0);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
