#include "CoreTypes.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"
#include "PCG/WFCAssembler.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWFCAssemblerTest, "Project.UnitTests.PCG.WFCAssembler", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)

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

    return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS

    // Test 2: Assembly with multiple modules
    FWFCModule Torso;
    Torso.ModuleId = "Torso";
    Modules.Add(Torso);

    FWFCAssembly Assembly2 = UWFCAssembler::AssembleWithWFC(Modules, 2, 2, 456);
    TestEqual("Assembly2 should have correct width", Assembly2.Width, 2);
    TestEqual("Assembly2 should have correct height", Assembly2.Height, 2);
    TestEqual("Assembly2 grid should have 4 elements", Assembly2.PlacedModules.Num(), 4);
