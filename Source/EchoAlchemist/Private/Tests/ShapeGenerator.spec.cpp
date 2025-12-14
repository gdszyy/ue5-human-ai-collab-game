#include "CoreTypes.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"
#include "PCG/ShapeGenerator.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShapeGeneratorTest, "Project.UnitTests.PCG.ShapeGenerator", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

bool FShapeGeneratorTest::RunTest(const FString& Parameters)
{
    // Test 1: Cellular Automata
    FShapeData CAShape = UShapeGenerator::GenerateShapeWithCellularAutomata(10, 10, 123);
    TestEqual("CA Shape should have correct width", CAShape.Width, 10);
    TestEqual("CA Shape should have correct height", CAShape.Height, 10);
    TestEqual("CA Shape grid should have 100 elements", CAShape.Grid.Num(), 100);

    // Test 2: Simplex Noise
    FShapeData NoiseShape = UShapeGenerator::GenerateShapeWithSimplexNoise(20, 20, 456);
    TestEqual("Noise Shape should have correct width", NoiseShape.Width, 20);
    TestEqual("Noise Shape should have correct height", NoiseShape.Height, 20);
    TestEqual("Noise Shape grid should have 400 elements", NoiseShape.Grid.Num(), 400);

    // Test 3: Voronoi
    FShapeData VoronoiShape = UShapeGenerator::GenerateShapeWithVoronoi(15, 15, 789);
    TestEqual("Voronoi Shape should have correct width", VoronoiShape.Width, 15);
    TestEqual("Voronoi Shape should have correct height", VoronoiShape.Height, 15);
    TestEqual("Voronoi Shape grid should have 225 elements", VoronoiShape.Grid.Num(), 225);

    // Test 4: Crystal Growth
    FShapeData CrystalShape = UShapeGenerator::GenerateShapeWithCrystalGrowth(25, 25, 101);
    TestEqual("Crystal Shape should have correct width", CrystalShape.Width, 25);
    TestEqual("Crystal Shape should have correct height", CrystalShape.Height, 25);
    TestEqual("Crystal Shape grid should have 625 elements", CrystalShape.Grid.Num(), 625);

    return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS
