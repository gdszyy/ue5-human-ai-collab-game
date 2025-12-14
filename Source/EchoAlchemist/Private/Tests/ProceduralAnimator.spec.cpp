#include "CoreTypes.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"
#include "PCG/ProceduralAnimator.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest, "Project.UnitTests.PCG.ProceduralAnimator", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

bool FProceduralAnimatorTest::RunTest(const FString& Parameters)
{
    // Test 1: Basic Animation Generation
    F2DSkeleton Skeleton;
    F2DBone Root;
    Root.BoneName = "Root";
    Skeleton.Bones.Add(Root);

    TArray<FAnimationFrame> Animation = UProceduralAnimator::GenerateWalkAnimation(Skeleton);
    TestTrue("Animation should have frames", Animation.Num() > 0);

    return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS
