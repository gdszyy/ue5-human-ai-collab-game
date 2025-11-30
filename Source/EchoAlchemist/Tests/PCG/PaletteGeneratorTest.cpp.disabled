// Copyright Echo Alchemist. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PCG/PaletteGenerator.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorFromSpectrumTest, "EchoAlchemist.PCG.PaletteGenerator.GenerateFromSpectrum", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorFromSpectrumTest::RunTest(const FString& Parameters)
{
    // Test: Generate palette from spectrum
    TArray<FLinearColor> Spectrum;
    Spectrum.Add(FLinearColor::Red);
    Spectrum.Add(FLinearColor::Blue);
    
    int32 NumColors = 5;
    FPalette Result = UPaletteGenerator::GeneratePaletteFromSpectrum(Spectrum, NumColors);
    
    // Verify: Should generate exactly NumColors colors
    TestEqual(TEXT("Generated palette should have correct number of colors"), Result.Colors.Num(), NumColors);
    
    // Verify: First and last colors should be close to spectrum endpoints
    if (Result.Colors.Num() > 0)
    {
        TestTrue(TEXT("First color should be close to Red"), Result.Colors[0].Equals(FLinearColor::Red, 0.1f));
        TestTrue(TEXT("Last color should be close to Blue"), Result.Colors[NumColors - 1].Equals(FLinearColor::Blue, 0.1f));
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorFromSpectrumEmptyTest, "EchoAlchemist.PCG.PaletteGenerator.GenerateFromSpectrum.Empty", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorFromSpectrumEmptyTest::RunTest(const FString& Parameters)
{
    // Test: Generate palette from empty spectrum
    TArray<FLinearColor> EmptySpectrum;
    int32 NumColors = 5;
    FPalette Result = UPaletteGenerator::GeneratePaletteFromSpectrum(EmptySpectrum, NumColors);
    
    // Verify: Should return empty palette
    TestEqual(TEXT("Empty spectrum should produce empty palette"), Result.Colors.Num(), 0);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorMonochromaticTest, "EchoAlchemist.PCG.PaletteGenerator.GenerateMonochromatic", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorMonochromaticTest::RunTest(const FString& Parameters)
{
    // Test: Generate monochromatic palette
    FLinearColor BaseColor = FLinearColor::Green;
    int32 NumColors = 5;
    FVector2D SaturationRange(0.2f, 1.0f);
    FVector2D ValueRange(0.3f, 1.0f);
    
    FPalette Result = UPaletteGenerator::GenerateMonochromaticPalette(BaseColor, NumColors, SaturationRange, ValueRange);
    
    // Verify: Should generate exactly NumColors colors
    TestEqual(TEXT("Generated monochromatic palette should have correct number of colors"), Result.Colors.Num(), NumColors);
    
    // Verify: All colors should have similar hue to base color
    if (Result.Colors.Num() > 0)
    {
        FLinearColor HSV_Base = BaseColor.LinearRGBToHSV();
        for (const FLinearColor& Color : Result.Colors)
        {
            FLinearColor HSV_Color = Color.LinearRGBToHSV();
            // Hue should be similar (within 10 degrees)
            float HueDiff = FMath::Abs(HSV_Color.R - HSV_Base.R);
            TestTrue(TEXT("All colors should have similar hue"), HueDiff < 10.0f || HueDiff > 350.0f);
        }
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPaletteGeneratorMonochromaticZeroColorsTest, "EchoAlchemist.PCG.PaletteGenerator.GenerateMonochromatic.ZeroColors", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FPaletteGeneratorMonochromaticZeroColorsTest::RunTest(const FString& Parameters)
{
    // Test: Generate monochromatic palette with zero colors
    FLinearColor BaseColor = FLinearColor::Green;
    int32 NumColors = 0;
    
    FPalette Result = UPaletteGenerator::GenerateMonochromaticPalette(BaseColor, NumColors);
    
    // Verify: Should return empty palette
    TestEqual(TEXT("Zero colors should produce empty palette"), Result.Colors.Num(), 0);
    
    return true;
}
