// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/WFCAssembler.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerTest_BasicAssembly, "EchoAlchemist.PCG.WFCAssembler.BasicAssembly", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerTest_BasicAssembly::RunTest(const FString& Parameters)
{
    // Test 1: Empty module list should return empty assembly
    {
        TArray<FWFCModule> EmptyModules;
        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(EmptyModules, 3, 3, 12345);
        
        TestEqual(TEXT("Empty modules should produce empty grid"), Assembly.Grid.Num(), 0);
        TestFalse(TEXT("Empty modules should not succeed"), Assembly.bSuccess);
    }

    // Test 2: Single module should fill entire grid
    {
        TArray<FWFCModule> SingleModule;
        FWFCModule Module;
        Module.ModuleID = TEXT("TestModule");
        Module.NorthConnectors.Add(TEXT("A"));
        Module.SouthConnectors.Add(TEXT("A"));
        Module.EastConnectors.Add(TEXT("A"));
        Module.WestConnectors.Add(TEXT("A"));
        SingleModule.Add(Module);

        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(SingleModule, 3, 3, 12345);
        
        TestEqual(TEXT("Single module should fill 3x3 grid"), Assembly.Grid.Num(), 9);
        TestTrue(TEXT("Single module assembly should succeed"), Assembly.bSuccess);
        
        // All cells should have the same module
        for (const FWFCCell& Cell : Assembly.Grid)
        {
            TestEqual(TEXT("All cells should use the same module"), Cell.ModuleID, TEXT("TestModule"));
        }
    }

    // Test 3: Grid dimensions should be correct
    {
        TArray<FWFCModule> Modules;
        FWFCModule Module;
        Module.ModuleID = TEXT("TestModule");
        Module.NorthConnectors.Add(TEXT("A"));
        Module.SouthConnectors.Add(TEXT("A"));
        Module.EastConnectors.Add(TEXT("A"));
        Module.WestConnectors.Add(TEXT("A"));
        Modules.Add(Module);

        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, 5, 4, 12345);
        
        TestEqual(TEXT("Width should be correct"), Assembly.Width, 5);
        TestEqual(TEXT("Height should be correct"), Assembly.Height, 4);
        TestEqual(TEXT("Grid size should match dimensions"), Assembly.Grid.Num(), 20);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerTest_ConnectorCompatibility, "EchoAlchemist.PCG.WFCAssembler.ConnectorCompatibility", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerTest_ConnectorCompatibility::RunTest(const FString& Parameters)
{
    // Test 1: Compatible modules should connect
    {
        TArray<FWFCModule> Modules;
        
        // Module A: North=X, South=Y, East=Z, West=W
        FWFCModule ModuleA;
        ModuleA.ModuleID = TEXT("A");
        ModuleA.NorthConnectors.Add(TEXT("X"));
        ModuleA.SouthConnectors.Add(TEXT("Y"));
        ModuleA.EastConnectors.Add(TEXT("Z"));
        ModuleA.WestConnectors.Add(TEXT("W"));
        Modules.Add(ModuleA);
        
        // Module B: North=Y, South=X, East=W, West=Z (compatible with A)
        FWFCModule ModuleB;
        ModuleB.ModuleID = TEXT("B");
        ModuleB.NorthConnectors.Add(TEXT("Y"));
        ModuleB.SouthConnectors.Add(TEXT("X"));
        ModuleB.EastConnectors.Add(TEXT("W"));
        ModuleB.WestConnectors.Add(TEXT("Z"));
        Modules.Add(ModuleB);

        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, 2, 2, 12345);
        
        TestTrue(TEXT("Compatible modules should assemble successfully"), Assembly.bSuccess);
        TestEqual(TEXT("Should fill 2x2 grid"), Assembly.Grid.Num(), 4);
    }

    // Test 2: Incompatible modules should fail or use backtracking
    {
        TArray<FWFCModule> Modules;
        
        // Module A: All connectors are "X"
        FWFCModule ModuleA;
        ModuleA.ModuleID = TEXT("A");
        ModuleA.NorthConnectors.Add(TEXT("X"));
        ModuleA.SouthConnectors.Add(TEXT("X"));
        ModuleA.EastConnectors.Add(TEXT("X"));
        ModuleA.WestConnectors.Add(TEXT("X"));
        Modules.Add(ModuleA);
        
        // Module B: All connectors are "Y" (incompatible with A)
        FWFCModule ModuleB;
        ModuleB.ModuleID = TEXT("B");
        ModuleB.NorthConnectors.Add(TEXT("Y"));
        ModuleB.SouthConnectors.Add(TEXT("Y"));
        ModuleB.EastConnectors.Add(TEXT("Y"));
        ModuleB.WestConnectors.Add(TEXT("Y"));
        Modules.Add(ModuleB);

        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, 2, 2, 12345);
        
        // Should either fail or only use one module type
        if (Assembly.bSuccess)
        {
            // If successful, all cells should use the same module
            FString FirstModuleID = Assembly.Grid[0].ModuleID;
            for (const FWFCCell& Cell : Assembly.Grid)
            {
                TestEqual(TEXT("All cells should use same module when incompatible"), Cell.ModuleID, FirstModuleID);
            }
        }
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerTest_Determinism, "EchoAlchemist.PCG.WFCAssembler.Determinism", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerTest_Determinism::RunTest(const FString& Parameters)
{
    // Test: Same seed should produce same result
    TArray<FWFCModule> Modules;
    
    FWFCModule ModuleA;
    ModuleA.ModuleID = TEXT("A");
    ModuleA.NorthConnectors.Add(TEXT("X"));
    ModuleA.SouthConnectors.Add(TEXT("X"));
    ModuleA.EastConnectors.Add(TEXT("X"));
    ModuleA.WestConnectors.Add(TEXT("X"));
    Modules.Add(ModuleA);
    
    FWFCModule ModuleB;
    ModuleB.ModuleID = TEXT("B");
    ModuleB.NorthConnectors.Add(TEXT("X"));
    ModuleB.SouthConnectors.Add(TEXT("X"));
    ModuleB.EastConnectors.Add(TEXT("X"));
    ModuleB.WestConnectors.Add(TEXT("X"));
    Modules.Add(ModuleB);

    const int32 TestSeed = 42;
    FWFCAssembly Assembly1 = UWFCAssembler::AssembleWithWFC(Modules, 4, 4, TestSeed);
    FWFCAssembly Assembly2 = UWFCAssembler::AssembleWithWFC(Modules, 4, 4, TestSeed);
    
    TestEqual(TEXT("Same seed should produce same success state"), Assembly1.bSuccess, Assembly2.bSuccess);
    
    if (Assembly1.bSuccess && Assembly2.bSuccess)
    {
        TestEqual(TEXT("Same seed should produce same grid size"), Assembly1.Grid.Num(), Assembly2.Grid.Num());
        
        for (int32 i = 0; i < Assembly1.Grid.Num(); ++i)
        {
            TestEqual(TEXT("Same seed should produce same module at each position"),
                     Assembly1.Grid[i].ModuleID, Assembly2.Grid[i].ModuleID);
        }
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerTest_GridCoordinates, "EchoAlchemist.PCG.WFCAssembler.GridCoordinates", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerTest_GridCoordinates::RunTest(const FString& Parameters)
{
    // Test: Grid coordinates should be correct
    TArray<FWFCModule> Modules;
    
    FWFCModule Module;
    Module.ModuleID = TEXT("TestModule");
    Module.NorthConnectors.Add(TEXT("A"));
    Module.SouthConnectors.Add(TEXT("A"));
    Module.EastConnectors.Add(TEXT("A"));
    Module.WestConnectors.Add(TEXT("A"));
    Modules.Add(Module);

    const int32 Width = 3;
    const int32 Height = 4;
    FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, Width, Height, 12345);
    
    TestTrue(TEXT("Assembly should succeed"), Assembly.bSuccess);
    TestEqual(TEXT("Grid should have correct size"), Assembly.Grid.Num(), Width * Height);
    
    // Check that all coordinates are within bounds
    for (const FWFCCell& Cell : Assembly.Grid)
    {
        TestTrue(TEXT("X coordinate should be within bounds"), Cell.X >= 0 && Cell.X < Width);
        TestTrue(TEXT("Y coordinate should be within bounds"), Cell.Y >= 0 && Cell.Y < Height);
    }
    
    // Check that all positions are unique
    TSet<FString> UniquePositions;
    for (const FWFCCell& Cell : Assembly.Grid)
    {
        FString Position = FString::Printf(TEXT("%d,%d"), Cell.X, Cell.Y);
        TestFalse(TEXT("Each position should be unique"), UniquePositions.Contains(Position));
        UniquePositions.Add(Position);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerTest_DifferentSeeds, "EchoAlchemist.PCG.WFCAssembler.DifferentSeeds", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerTest_DifferentSeeds::RunTest(const FString& Parameters)
{
    // Test: Different seeds should produce different results (with high probability)
    TArray<FWFCModule> Modules;
    
    // Create multiple compatible modules
    for (int32 i = 0; i < 5; ++i)
    {
        FWFCModule Module;
        Module.ModuleID = FString::Printf(TEXT("Module%d"), i);
        Module.NorthConnectors.Add(TEXT("A"));
        Module.SouthConnectors.Add(TEXT("A"));
        Module.EastConnectors.Add(TEXT("A"));
        Module.WestConnectors.Add(TEXT("A"));
        Modules.Add(Module);
    }

    FWFCAssembly Assembly1 = UWFCAssembler::AssembleWithWFC(Modules, 5, 5, 111);
    FWFCAssembly Assembly2 = UWFCAssembler::AssembleWithWFC(Modules, 5, 5, 222);
    FWFCAssembly Assembly3 = UWFCAssembler::AssembleWithWFC(Modules, 5, 5, 333);
    
    // Count differences
    int32 DifferenceCount12 = 0;
    int32 DifferenceCount23 = 0;
    
    if (Assembly1.bSuccess && Assembly2.bSuccess && Assembly3.bSuccess)
    {
        for (int32 i = 0; i < Assembly1.Grid.Num(); ++i)
        {
            if (Assembly1.Grid[i].ModuleID != Assembly2.Grid[i].ModuleID)
            {
                DifferenceCount12++;
            }
            if (Assembly2.Grid[i].ModuleID != Assembly3.Grid[i].ModuleID)
            {
                DifferenceCount23++;
            }
        }
        
        // With 5 module types and 25 cells, we expect some variation
        // (This test might occasionally fail due to randomness, but probability is low)
        TestTrue(TEXT("Different seeds should produce some variation"),
                 DifferenceCount12 > 0 || DifferenceCount23 > 0);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerTest_EdgeCases, "EchoAlchemist.PCG.WFCAssembler.EdgeCases", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWFCAssemblerTest_EdgeCases::RunTest(const FString& Parameters)
{
    TArray<FWFCModule> Modules;
    
    FWFCModule Module;
    Module.ModuleID = TEXT("TestModule");
    Module.NorthConnectors.Add(TEXT("A"));
    Module.SouthConnectors.Add(TEXT("A"));
    Module.EastConnectors.Add(TEXT("A"));
    Module.WestConnectors.Add(TEXT("A"));
    Modules.Add(Module);

    // Test 1: 1x1 grid
    {
        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, 1, 1, 12345);
        TestTrue(TEXT("1x1 grid should succeed"), Assembly.bSuccess);
        TestEqual(TEXT("1x1 grid should have 1 cell"), Assembly.Grid.Num(), 1);
    }

    // Test 2: 1xN grid (vertical line)
    {
        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, 1, 10, 12345);
        TestTrue(TEXT("1x10 grid should succeed"), Assembly.bSuccess);
        TestEqual(TEXT("1x10 grid should have 10 cells"), Assembly.Grid.Num(), 10);
    }

    // Test 3: Nx1 grid (horizontal line)
    {
        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, 10, 1, 12345);
        TestTrue(TEXT("10x1 grid should succeed"), Assembly.bSuccess);
        TestEqual(TEXT("10x1 grid should have 10 cells"), Assembly.Grid.Num(), 10);
    }

    // Test 4: Large grid
    {
        FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, 20, 20, 12345);
        TestTrue(TEXT("20x20 grid should succeed"), Assembly.bSuccess);
        TestEqual(TEXT("20x20 grid should have 400 cells"), Assembly.Grid.Num(), 400);
    }

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
