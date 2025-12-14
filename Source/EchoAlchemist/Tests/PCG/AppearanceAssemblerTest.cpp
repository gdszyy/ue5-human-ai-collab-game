// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/AppearanceAssembler.h"
#include "PCG/AttributeGenerator.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAppearanceAssemblerTest_CalculateMonsterScale, "EchoAlchemist.PCG.AppearanceAssembler.CalculateMonsterScale", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAppearanceAssemblerTest_CalculateMonsterScale::RunTest(const FString& Parameters)
{
    // Test 1: Tiny size should have smallest scale
    {
        float TinyScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Tiny, 1.0f);
        TestEqual(TEXT("Tiny scale should be 0.5"), TinyScale, 0.5f);
    }

    // Test 2: Small size should be 0.75
    {
        float SmallScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Small, 1.0f);
        TestEqual(TEXT("Small scale should be 0.75"), SmallScale, 0.75f);
    }

    // Test 3: Medium size should be 1.0 (base)
    {
        float MediumScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Medium, 1.0f);
        TestEqual(TEXT("Medium scale should be 1.0"), MediumScale, 1.0f);
    }

    // Test 4: Large size should be 1.5
    {
        float LargeScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Large, 1.0f);
        TestEqual(TEXT("Large scale should be 1.5"), LargeScale, 1.5f);
    }

    // Test 5: Giant size should be largest
    {
        float GiantScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Giant, 1.0f);
        TestEqual(TEXT("Giant scale should be 2.5"), GiantScale, 2.5f);
    }

    // Test 6: Base scale should be multiplied correctly
    {
        float ScaledGiant = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Giant, 2.0f);
        TestEqual(TEXT("Giant with base scale 2.0 should be 5.0"), ScaledGiant, 5.0f);
    }

    // Test 7: Scales should be in ascending order
    {
        float TinyScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Tiny, 1.0f);
        float SmallScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Small, 1.0f);
        float MediumScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Medium, 1.0f);
        float LargeScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Large, 1.0f);
        float GiantScale = UAppearanceAssembler::CalculateMonsterScale(ESizeClass::Giant, 1.0f);
        
        TestTrue(TEXT("Tiny < Small"), TinyScale < SmallScale);
        TestTrue(TEXT("Small < Medium"), SmallScale < MediumScale);
        TestTrue(TEXT("Medium < Large"), MediumScale < LargeScale);
        TestTrue(TEXT("Large < Giant"), LargeScale < GiantScale);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAppearanceAssemblerTest_GetAnchorPointLocation, "EchoAlchemist.PCG.AppearanceAssembler.GetAnchorPointLocation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAppearanceAssemblerTest_GetAnchorPointLocation::RunTest(const FString& Parameters)
{
    FSkeletonData DummySkeletonData;
    
    // Test 1: Head anchor should be at top
    {
        FVector HeadLocation = UAppearanceAssembler::GetAnchorPointLocation(TEXT("Head"), DummySkeletonData);
        TestTrue(TEXT("Head anchor should have positive Z"), HeadLocation.Z > 0.0f);
    }

    // Test 2: Tail anchor should be at back
    {
        FVector TailLocation = UAppearanceAssembler::GetAnchorPointLocation(TEXT("Tail"), DummySkeletonData);
        TestTrue(TEXT("Tail anchor should have negative X"), TailLocation.X < 0.0f);
    }

    // Test 3: Back anchor should be at center-top
    {
        FVector BackLocation = UAppearanceAssembler::GetAnchorPointLocation(TEXT("Back"), DummySkeletonData);
        TestTrue(TEXT("Back anchor should have positive Z"), BackLocation.Z > 0.0f);
        TestEqual(TEXT("Back anchor should have zero X"), BackLocation.X, 0.0f);
    }

    // Test 4: Unknown anchor should return zero vector
    {
        FVector UnknownLocation = UAppearanceAssembler::GetAnchorPointLocation(TEXT("UnknownAnchor"), DummySkeletonData);
        TestEqual(TEXT("Unknown anchor should return zero vector"), UnknownLocation, FVector::ZeroVector);
    }

    // Test 5: Left and right anchors should be symmetric
    {
        FVector FrontLeft = UAppearanceAssembler::GetAnchorPointLocation(TEXT("FrontLeft"), DummySkeletonData);
        FVector FrontRight = UAppearanceAssembler::GetAnchorPointLocation(TEXT("FrontRight"), DummySkeletonData);
        
        TestEqual(TEXT("Left and right should have same X"), FrontLeft.X, FrontRight.X);
        TestEqual(TEXT("Left and right should have same Z"), FrontLeft.Z, FrontRight.Z);
        TestTrue(TEXT("Left should have negative Y"), FrontLeft.Y < 0.0f);
        TestTrue(TEXT("Right should have positive Y"), FrontRight.Y > 0.0f);
        TestEqual(TEXT("Left and right Y should be symmetric"), FrontLeft.Y, -FrontRight.Y);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAppearanceAssemblerTest_GetPaletteForHabitat, "EchoAlchemist.PCG.AppearanceAssembler.GetPaletteForHabitat", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAppearanceAssemblerTest_GetPaletteForHabitat::RunTest(const FString& Parameters)
{
    // Test without data table (should use fallback defaults)
    
    // Test 1: Forest palette should be green
    {
        FPaletteData ForestPalette;
        bool bSuccess = UAppearanceAssembler::GetPaletteForHabitat(EHabitatType::Forest, nullptr, ForestPalette);
        
        TestTrue(TEXT("Should succeed with fallback palette"), bSuccess);
        TestEqual(TEXT("Forest habitat should be set"), ForestPalette.Habitat, EHabitatType::Forest);
        TestTrue(TEXT("Forest primary color should be greenish"), ForestPalette.PrimaryColor.G > 0.5f);
    }

    // Test 2: Desert palette should be yellow/brown
    {
        FPaletteData DesertPalette;
        bool bSuccess = UAppearanceAssembler::GetPaletteForHabitat(EHabitatType::Desert, nullptr, DesertPalette);
        
        TestTrue(TEXT("Should succeed with fallback palette"), bSuccess);
        TestEqual(TEXT("Desert habitat should be set"), DesertPalette.Habitat, EHabitatType::Desert);
        TestTrue(TEXT("Desert primary color should be warm"), 
                 DesertPalette.PrimaryColor.R > 0.5f && DesertPalette.PrimaryColor.G > 0.5f);
    }

    // Test 3: Tundra palette should be blue/white
    {
        FPaletteData TundraPalette;
        bool bSuccess = UAppearanceAssembler::GetPaletteForHabitat(EHabitatType::Tundra, nullptr, TundraPalette);
        
        TestTrue(TEXT("Should succeed with fallback palette"), bSuccess);
        TestEqual(TEXT("Tundra habitat should be set"), TundraPalette.Habitat, EHabitatType::Tundra);
        TestTrue(TEXT("Tundra primary color should be cold"), TundraPalette.PrimaryColor.B > 0.5f);
    }

    // Test 4: Volcano palette should be red/orange
    {
        FPaletteData VolcanoPalette;
        bool bSuccess = UAppearanceAssembler::GetPaletteForHabitat(EHabitatType::Volcano, nullptr, VolcanoPalette);
        
        TestTrue(TEXT("Should succeed with fallback palette"), bSuccess);
        TestEqual(TEXT("Volcano habitat should be set"), VolcanoPalette.Habitat, EHabitatType::Volcano);
        TestTrue(TEXT("Volcano primary color should be hot"), VolcanoPalette.PrimaryColor.R > 0.8f);
    }

    // Test 5: Cave palette should be gray
    {
        FPaletteData CavePalette;
        bool bSuccess = UAppearanceAssembler::GetPaletteForHabitat(EHabitatType::Cave, nullptr, CavePalette);
        
        TestTrue(TEXT("Should succeed with fallback palette"), bSuccess);
        TestEqual(TEXT("Cave habitat should be set"), CavePalette.Habitat, EHabitatType::Cave);
        // Gray colors have similar R, G, B values
        float ColorDiff = FMath::Abs(CavePalette.PrimaryColor.R - CavePalette.PrimaryColor.G) + 
                         FMath::Abs(CavePalette.PrimaryColor.G - CavePalette.PrimaryColor.B);
        TestTrue(TEXT("Cave primary color should be grayish"), ColorDiff < 0.3f);
    }

    // Test 6: Swamp palette should be murky green
    {
        FPaletteData SwampPalette;
        bool bSuccess = UAppearanceAssembler::GetPaletteForHabitat(EHabitatType::Swamp, nullptr, SwampPalette);
        
        TestTrue(TEXT("Should succeed with fallback palette"), bSuccess);
        TestEqual(TEXT("Swamp habitat should be set"), SwampPalette.Habitat, EHabitatType::Swamp);
        TestTrue(TEXT("Swamp primary color should be greenish"), SwampPalette.PrimaryColor.G > 0.5f);
    }

    // Test 7: All palettes should have valid colors
    {
        TArray<EHabitatType> AllHabitats = {
            EHabitatType::Forest, EHabitatType::Desert, EHabitatType::Tundra,
            EHabitatType::Cave, EHabitatType::Volcano, EHabitatType::Swamp
        };
        
        for (EHabitatType Habitat : AllHabitats)
        {
            FPaletteData Palette;
            UAppearanceAssembler::GetPaletteForHabitat(Habitat, nullptr, Palette);
            
            TestTrue(TEXT("Primary color should be valid"), Palette.PrimaryColor.A > 0.0f);
            TestTrue(TEXT("Secondary color should be valid"), Palette.SecondaryColor.A > 0.0f);
            TestTrue(TEXT("Accent color should be valid"), Palette.AccentColor.A > 0.0f);
        }
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAppearanceAssemblerTest_CreatePaletteSwapMaterial, "EchoAlchemist.PCG.AppearanceAssembler.CreatePaletteSwapMaterial", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAppearanceAssemblerTest_CreatePaletteSwapMaterial::RunTest(const FString& Parameters)
{
    // Test 1: Null base material should return nullptr
    {
        FPaletteData TestPalette;
        TestPalette.PrimaryColor = FLinearColor::Red;
        TestPalette.SecondaryColor = FLinearColor::Green;
        TestPalette.AccentColor = FLinearColor::Blue;
        
        UMaterialInstanceDynamic* Result = UAppearanceAssembler::CreatePaletteSwapMaterial(nullptr, TestPalette);
        TestNull(TEXT("Null base material should return nullptr"), Result);
    }

    // Note: Testing with actual materials requires a valid UMaterialInterface,
    // which is difficult to create in a unit test without loading assets.
    // In a real project, this would be tested with integration tests that load actual materials.

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAppearanceAssemblerTest_Integration, "EchoAlchemist.PCG.AppearanceAssembler.Integration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAppearanceAssemblerTest_Integration::RunTest(const FString& Parameters)
{
    // Integration test: Generate monster attributes and verify appearance can be assembled
    
    // Test 1: Generate a forest monster and get its palette
    {
        FMonsterAttributes Monster = UAttributeGenerator::GenerateMonsterAttributes(12345, 0.5f, 0.5f, 2);
        
        FPaletteData Palette;
        bool bPaletteSuccess = UAppearanceAssembler::GetPaletteForHabitat(
            Monster.EcologyAttributes.Habitat, nullptr, Palette);
        
        TestTrue(TEXT("Should get palette for generated habitat"), bPaletteSuccess);
        TestEqual(TEXT("Palette habitat should match monster habitat"), 
                  Palette.Habitat, Monster.EcologyAttributes.Habitat);
    }

    // Test 2: Calculate scale for generated monster
    {
        FMonsterAttributes Monster = UAttributeGenerator::GenerateMonsterAttributes(12346, 0.8f, 0.2f, 2);
        
        float Scale = UAppearanceAssembler::CalculateMonsterScale(Monster.EcologyAttributes.SizeClass, 1.0f);
        
        TestTrue(TEXT("Scale should be positive"), Scale > 0.0f);
        TestTrue(TEXT("Scale should be reasonable"), Scale >= 0.5f && Scale <= 3.0f);
    }

    // Test 3: Verify different monsters get different appearances
    {
        FMonsterAttributes Monster1 = UAttributeGenerator::GenerateMonsterAttributes(11111, 0.9f, 0.1f, 2);
        FMonsterAttributes Monster2 = UAttributeGenerator::GenerateMonsterAttributes(22222, 0.1f, 0.9f, 2);
        
        FPaletteData Palette1, Palette2;
        UAppearanceAssembler::GetPaletteForHabitat(Monster1.EcologyAttributes.Habitat, nullptr, Palette1);
        UAppearanceAssembler::GetPaletteForHabitat(Monster2.EcologyAttributes.Habitat, nullptr, Palette2);
        
        float Scale1 = UAppearanceAssembler::CalculateMonsterScale(Monster1.EcologyAttributes.SizeClass, 1.0f);
        float Scale2 = UAppearanceAssembler::CalculateMonsterScale(Monster2.EcologyAttributes.SizeClass, 1.0f);
        
        // Different seeds should likely produce different appearances
        bool bDifferentAppearance = (Monster1.EcologyAttributes.Habitat != Monster2.EcologyAttributes.Habitat) ||
                                   (Monster1.EcologyAttributes.SizeClass != Monster2.EcologyAttributes.SizeClass) ||
                                   (Scale1 != Scale2);
        
        TestTrue(TEXT("Different seeds should produce different appearances"), bDifferentAppearance);
    }

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
