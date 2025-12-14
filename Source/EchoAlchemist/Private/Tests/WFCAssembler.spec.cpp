#include "CoreTypes.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"
#include "PCG/WFCAssembler.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerTest, "Project.UnitTests.PCG.WFCAssembler", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

bool FWFCAssemblerTest::RunTest(const FString& Parameters)
{
    // Test 1: Basic Assembly
    TArray<FWFCModule> Modules;
    FWFCModule Head;
    Head.ModuleId = "Head";
    Modules.Add(Head);

    FWFCAssembly Assembly = UWFCAssembler::AssembleWithWFC(Modules, 3, 3, 123);
    TestEqual("Assembly should have correct width", Assembly.Width, 3);
    TestEqual("Assembly should have correct height", Assembly.Height, 3);
    TestEqual("Assembly grid should have 9 elements", Assembly.PlacedModules.Num(), 9);

    // Test 2: Assembly with multiple modules
    FWFCModule Torso;
    Torso.ModuleId = "Torso";
    Modules.Add(Torso);

    FWFCAssembly Assembly2 = UWFCAssembler::AssembleWithWFC(Modules, 2, 2, 456);
    TestEqual("Assembly2 should have correct width", Assembly2.Width, 2);
    TestEqual("Assembly2 should have correct height", Assembly2.Height, 2);
    TestEqual("Assembly2 grid should have 4 elements", Assembly2.PlacedModules.Num(), 4);

    // Test 3: Assembly with constraints
    TArray<FWFCModule> ConstrainedModules;
    FWFCModule Top, Bottom;
    Top.ModuleId = "Top";
    
    // Create connector for Top module
    FWFCConnector TopConnector;
    TopConnector.ConnectorType = "socket";
    TopConnector.Direction = "South";
    Top.Connectors.Add(TopConnector);
    
    Bottom.ModuleId = "Bottom";
    
    // Create connector for Bottom module
    FWFCConnector BottomConnector;
    BottomConnector.ConnectorType = "socket";
    BottomConnector.Direction = "North";
    Bottom.Connectors.Add(BottomConnector);
    
    ConstrainedModules.Add(Top);
    ConstrainedModules.Add(Bottom);

    FWFCAssembly Assembly3 = UWFCAssembler::AssembleWithWFC(ConstrainedModules, 1, 2, 789);
    TestEqual("Constrained assembly should have Top at (0,0)", Assembly3.PlacedModules[0], FName("Top"));
    TestEqual("Constrained assembly should have Bottom at (0,1)", Assembly3.PlacedModules[1], FName("Bottom"));

    return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS
