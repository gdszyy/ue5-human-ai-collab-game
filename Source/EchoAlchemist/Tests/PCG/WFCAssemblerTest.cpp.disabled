// Copyright Echo Alchemist. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PCG/WFCAssembler.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerBasicTest, "EchoAlchemist.PCG.WFCAssembler.AssembleParts", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerBasicTest::RunTest(const FString& Parameters)
{
    // Test: Assemble parts using WFC algorithm
    TArray<FWFCPart> AvailableParts;
    
    // Create test parts
    FWFCPart Part1;
    Part1.PartName = TEXT("Head");
    Part1.PartType = EWFCPartType::Head;
    Part1.Weight = 1.0f;
    AvailableParts.Add(Part1);
    
    FWFCPart Part2;
    Part2.PartName = TEXT("Body");
    Part2.PartType = EWFCPartType::Body;
    Part2.Weight = 1.0f;
    AvailableParts.Add(Part2);
    
    FWFCPart Part3;
    Part3.PartName = TEXT("Legs");
    Part3.PartType = EWFCPartType::Legs;
    Part3.Weight = 1.0f;
    AvailableParts.Add(Part3);
    
    int32 Seed = 12345;
    TArray<FWFCPart> Result = UWFCAssembler::AssembleParts(AvailableParts, Seed);
    
    // Verify: Should return at least one part
    TestTrue(TEXT("WFC should assemble at least one part"), Result.Num() > 0);
    
    // Verify: All returned parts should be from the available parts
    for (const FWFCPart& Part : Result)
    {
        bool Found = false;
        for (const FWFCPart& AvailablePart : AvailableParts)
        {
            if (Part.PartName == AvailablePart.PartName)
            {
                Found = true;
                break;
            }
        }
        TestTrue(TEXT("All assembled parts should be from available parts"), Found);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerEmptyPartsTest, "EchoAlchemist.PCG.WFCAssembler.AssembleParts.Empty", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerEmptyPartsTest::RunTest(const FString& Parameters)
{
    // Test: Assemble with empty parts list
    TArray<FWFCPart> EmptyParts;
    int32 Seed = 12345;
    
    TArray<FWFCPart> Result = UWFCAssembler::AssembleParts(EmptyParts, Seed);
    
    // Verify: Should return empty array
    TestEqual(TEXT("Empty parts should produce empty assembly"), Result.Num(), 0);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerDeterministicTest, "EchoAlchemist.PCG.WFCAssembler.Deterministic", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerDeterministicTest::RunTest(const FString& Parameters)
{
    // Test: Same seed should produce same result
    TArray<FWFCPart> AvailableParts;
    
    // Create test parts
    for (int32 i = 0; i < 5; i++)
    {
        FWFCPart Part;
        Part.PartName = FString::Printf(TEXT("Part%d"), i);
        Part.PartType = EWFCPartType::Body;
        Part.Weight = 1.0f;
        AvailableParts.Add(Part);
    }
    
    int32 Seed = 54321;
    TArray<FWFCPart> Result1 = UWFCAssembler::AssembleParts(AvailableParts, Seed);
    TArray<FWFCPart> Result2 = UWFCAssembler::AssembleParts(AvailableParts, Seed);
    
    // Verify: Same seed should produce same result
    TestEqual(TEXT("Same seed should produce same number of parts"), Result1.Num(), Result2.Num());
    
    if (Result1.Num() == Result2.Num())
    {
        for (int32 i = 0; i < Result1.Num(); i++)
        {
            TestEqual(TEXT("Same seed should produce same parts"), Result1[i].PartName, Result2[i].PartName);
        }
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerConstraintTest, "EchoAlchemist.PCG.WFCAssembler.ValidateConstraints", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerConstraintTest::RunTest(const FString& Parameters)
{
    // Test: Validate part constraints
    TArray<FWFCPart> Parts;
    
    FWFCPart Part1;
    Part1.PartName = TEXT("TestPart");
    Part1.PartType = EWFCPartType::Head;
    Part1.AllowedNeighbors.Add(TEXT("BodyPart"));
    Parts.Add(Part1);
    
    FWFCPart Part2;
    Part2.PartName = TEXT("BodyPart");
    Part2.PartType = EWFCPartType::Body;
    Parts.Add(Part2);
    
    bool Result = UWFCAssembler::ValidateConstraints(Parts);
    
    // Verify: Should validate successfully if constraints are met
    TestTrue(TEXT("Valid constraints should pass validation"), Result || Parts.Num() < 2);
    
    return true;
}
