// Copyright Echo Alchemist. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PCG/ShapeGenerator.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShapeGeneratorCircleTest, "EchoAlchemist.PCG.ShapeGenerator.GenerateCircle", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FShapeGeneratorCircleTest::RunTest(const FString& Parameters)
{
    // Test: Generate circle shape
    int32 Resolution = 8;
    float Radius = 100.0f;
    
    TArray<FVector2D> Result = UShapeGenerator::GenerateCircleShape(Resolution, Radius);
    
    // Verify: Should generate exactly Resolution points
    TestEqual(TEXT("Circle should have correct number of points"), Result.Num(), Resolution);
    
    // Verify: All points should be approximately at the specified radius
    for (const FVector2D& Point : Result)
    {
        float Distance = Point.Size();
        TestTrue(TEXT("All points should be at the specified radius"), FMath::IsNearlyEqual(Distance, Radius, 0.1f));
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShapeGeneratorCircleZeroResolutionTest, "EchoAlchemist.PCG.ShapeGenerator.GenerateCircle.ZeroResolution", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FShapeGeneratorCircleZeroResolutionTest::RunTest(const FString& Parameters)
{
    // Test: Generate circle with zero resolution
    int32 Resolution = 0;
    float Radius = 100.0f;
    
    TArray<FVector2D> Result = UShapeGenerator::GenerateCircleShape(Resolution, Radius);
    
    // Verify: Should return empty array
    TestEqual(TEXT("Zero resolution should produce empty shape"), Result.Num(), 0);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShapeGeneratorRectangleTest, "EchoAlchemist.PCG.ShapeGenerator.GenerateRectangle", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FShapeGeneratorRectangleTest::RunTest(const FString& Parameters)
{
    // Test: Generate rectangle shape
    FVector2D Size(200.0f, 100.0f);
    
    TArray<FVector2D> Result = UShapeGenerator::GenerateRectangleShape(Size);
    
    // Verify: Rectangle should have 4 corners
    TestEqual(TEXT("Rectangle should have 4 corners"), Result.Num(), 4);
    
    // Verify: Points should be at the corners
    if (Result.Num() == 4)
    {
        float HalfWidth = Size.X / 2.0f;
        float HalfHeight = Size.Y / 2.0f;
        
        TestTrue(TEXT("Corner 1 should be at top-right"), Result[0].Equals(FVector2D(HalfWidth, HalfHeight), 0.1f));
        TestTrue(TEXT("Corner 2 should be at top-left"), Result[1].Equals(FVector2D(-HalfWidth, HalfHeight), 0.1f));
        TestTrue(TEXT("Corner 3 should be at bottom-left"), Result[2].Equals(FVector2D(-HalfWidth, -HalfHeight), 0.1f));
        TestTrue(TEXT("Corner 4 should be at bottom-right"), Result[3].Equals(FVector2D(HalfWidth, -HalfHeight), 0.1f));
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShapeGeneratorPolygonTest, "EchoAlchemist.PCG.ShapeGenerator.GeneratePolygon", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FShapeGeneratorPolygonTest::RunTest(const FString& Parameters)
{
    // Test: Generate regular polygon (hexagon)
    int32 Sides = 6;
    float Radius = 100.0f;
    
    TArray<FVector2D> Result = UShapeGenerator::GeneratePolygonShape(Sides, Radius);
    
    // Verify: Should generate exactly Sides points
    TestEqual(TEXT("Polygon should have correct number of sides"), Result.Num(), Sides);
    
    // Verify: All points should be approximately at the specified radius
    for (const FVector2D& Point : Result)
    {
        float Distance = Point.Size();
        TestTrue(TEXT("All points should be at the specified radius"), FMath::IsNearlyEqual(Distance, Radius, 0.1f));
    }
    
    return true;
}
