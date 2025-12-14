// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/ProceduralAnimator.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest_IdleAnimation, "EchoAlchemist.PCG.ProceduralAnimator.IdleAnimation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorTest_IdleAnimation::RunTest(const FString& Parameters)
{
    // Create a simple test skeleton
    F2DSkeleton TestSkeleton;
    
    F2DBone RootBone;
    RootBone.BoneName = TEXT("Root");
    RootBone.ParentIndex = -1;
    RootBone.RestTransform = FTransform::Identity;
    TestSkeleton.Bones.Add(RootBone);
    
    F2DBone BodyBone;
    BodyBone.BoneName = TEXT("Body");
    BodyBone.ParentIndex = 0;
    BodyBone.RestTransform = FTransform(FVector(0, 0, 50));
    TestSkeleton.Bones.Add(BodyBone);

    // Generate idle animation
    TArray<FAnimationFrame> IdleAnimation = UProceduralAnimator::GenerateIdleAnimation(TestSkeleton, 1.0f);
    
    // Test 1: Animation should have frames
    TestTrue(TEXT("Idle animation should have frames"), IdleAnimation.Num() > 0);
    
    // Test 2: Each frame should have transforms for all bones
    for (const FAnimationFrame& Frame : IdleAnimation)
    {
        TestEqual(TEXT("Each frame should have transforms for all bones"),
                 Frame.BoneTransforms.Num(), TestSkeleton.Bones.Num());
    }
    
    // Test 3: Animation should loop (first and last frame should be similar)
    if (IdleAnimation.Num() >= 2)
    {
        const FAnimationFrame& FirstFrame = IdleAnimation[0];
        const FAnimationFrame& LastFrame = IdleAnimation[IdleAnimation.Num() - 1];
        
        // Check that root bone position is similar (allowing for small differences)
        FVector FirstPos = FirstFrame.BoneTransforms[0].GetLocation();
        FVector LastPos = LastFrame.BoneTransforms[0].GetLocation();
        float Distance = FVector::Dist(FirstPos, LastPos);
        
        TestTrue(TEXT("Idle animation should loop smoothly"), Distance < 10.0f);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest_WalkAnimation, "EchoAlchemist.PCG.ProceduralAnimator.WalkAnimation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorTest_WalkAnimation::RunTest(const FString& Parameters)
{
    // Create a skeleton with legs
    F2DSkeleton TestSkeleton;
    
    F2DBone RootBone;
    RootBone.BoneName = TEXT("Root");
    RootBone.ParentIndex = -1;
    RootBone.RestTransform = FTransform::Identity;
    TestSkeleton.Bones.Add(RootBone);
    
    F2DBone LeftLeg;
    LeftLeg.BoneName = TEXT("LeftLeg");
    LeftLeg.ParentIndex = 0;
    LeftLeg.RestTransform = FTransform(FVector(-20, 0, 0));
    TestSkeleton.Bones.Add(LeftLeg);
    
    F2DBone RightLeg;
    RightLeg.BoneName = TEXT("RightLeg");
    RightLeg.ParentIndex = 0;
    RightLeg.RestTransform = FTransform(FVector(20, 0, 0));
    TestSkeleton.Bones.Add(RightLeg);

    // Generate walk animation
    TArray<FAnimationFrame> WalkAnimation = UProceduralAnimator::GenerateWalkAnimation(TestSkeleton, 1.0f);
    
    // Test 1: Animation should have frames
    TestTrue(TEXT("Walk animation should have frames"), WalkAnimation.Num() > 0);
    
    // Test 2: Animation should have reasonable number of frames (8-12)
    TestTrue(TEXT("Walk animation should have 8-12 frames"),
             WalkAnimation.Num() >= 8 && WalkAnimation.Num() <= 12);
    
    // Test 3: Each frame should have transforms for all bones
    for (const FAnimationFrame& Frame : WalkAnimation)
    {
        TestEqual(TEXT("Each frame should have transforms for all bones"),
                 Frame.BoneTransforms.Num(), TestSkeleton.Bones.Num());
    }
    
    // Test 4: Legs should move (check that leg positions vary)
    if (WalkAnimation.Num() >= 2)
    {
        FVector FirstLeftLegPos = WalkAnimation[0].BoneTransforms[1].GetLocation();
        FVector MidLeftLegPos = WalkAnimation[WalkAnimation.Num() / 2].BoneTransforms[1].GetLocation();
        
        float LegMovement = FVector::Dist(FirstLeftLegPos, MidLeftLegPos);
        TestTrue(TEXT("Legs should move during walk animation"), LegMovement > 5.0f);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest_AttackAnimation, "EchoAlchemist.PCG.ProceduralAnimator.AttackAnimation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorTest_AttackAnimation::RunTest(const FString& Parameters)
{
    // Create a simple skeleton
    F2DSkeleton TestSkeleton;
    
    F2DBone RootBone;
    RootBone.BoneName = TEXT("Root");
    RootBone.ParentIndex = -1;
    RootBone.RestTransform = FTransform::Identity;
    TestSkeleton.Bones.Add(RootBone);
    
    F2DBone AttackLimb;
    AttackLimb.BoneName = TEXT("RightArm");
    AttackLimb.ParentIndex = 0;
    AttackLimb.RestTransform = FTransform(FVector(30, 0, 50));
    TestSkeleton.Bones.Add(AttackLimb);

    // Generate attack animation
    TArray<FAnimationFrame> AttackAnimation = UProceduralAnimator::GenerateAttackAnimation(TestSkeleton, FVector(1, 0, 0), 1.0f);
    
    // Test 1: Animation should have frames
    TestTrue(TEXT("Attack animation should have frames"), AttackAnimation.Num() > 0);
    
    // Test 2: Animation should have reasonable number of frames (10-16)
    TestTrue(TEXT("Attack animation should have 10-16 frames"),
             AttackAnimation.Num() >= 10 && AttackAnimation.Num() <= 16);
    
    // Test 3: Each frame should have transforms for all bones
    for (const FAnimationFrame& Frame : AttackAnimation)
    {
        TestEqual(TEXT("Each frame should have transforms for all bones"),
                 Frame.BoneTransforms.Num(), TestSkeleton.Bones.Num());
    }
    
    // Test 4: Attack limb should move forward significantly
    if (AttackAnimation.Num() >= 3)
    {
        FVector StartPos = AttackAnimation[0].BoneTransforms[1].GetLocation();
        FVector PeakPos = AttackAnimation[AttackAnimation.Num() / 2].BoneTransforms[1].GetLocation();
        
        // Attack should move forward (positive X direction)
        TestTrue(TEXT("Attack limb should move forward"), PeakPos.X > StartPos.X + 10.0f);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest_DeathAnimation, "EchoAlchemist.PCG.ProceduralAnimator.DeathAnimation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorTest_DeathAnimation::RunTest(const FString& Parameters)
{
    // Create a simple skeleton
    F2DSkeleton TestSkeleton;
    
    F2DBone RootBone;
    RootBone.BoneName = TEXT("Root");
    RootBone.ParentIndex = -1;
    RootBone.RestTransform = FTransform::Identity;
    TestSkeleton.Bones.Add(RootBone);
    
    F2DBone BodyBone;
    BodyBone.BoneName = TEXT("Body");
    BodyBone.ParentIndex = 0;
    BodyBone.RestTransform = FTransform(FVector(0, 0, 50));
    TestSkeleton.Bones.Add(BodyBone);

    // Generate death animation
    FVector FallDirection = FVector(1, 0, 0); // Fall to the right
    TArray<FAnimationFrame> DeathAnimation = UProceduralAnimator::GenerateDeathAnimation(TestSkeleton, FallDirection);
    
    // Test 1: Animation should have frames
    TestTrue(TEXT("Death animation should have frames"), DeathAnimation.Num() > 0);
    
    // Test 2: Animation should have reasonable number of frames (8-12)
    TestTrue(TEXT("Death animation should have 8-12 frames"),
             DeathAnimation.Num() >= 8 && DeathAnimation.Num() <= 12);
    
    // Test 3: Each frame should have transforms for all bones
    for (const FAnimationFrame& Frame : DeathAnimation)
    {
        TestEqual(TEXT("Each frame should have transforms for all bones"),
                 Frame.BoneTransforms.Num(), TestSkeleton.Bones.Num());
    }
    
    // Test 4: Body should fall down (Z position should decrease)
    if (DeathAnimation.Num() >= 2)
    {
        FVector StartPos = DeathAnimation[0].BoneTransforms[0].GetLocation();
        FVector EndPos = DeathAnimation[DeathAnimation.Num() - 1].BoneTransforms[0].GetLocation();
        
        TestTrue(TEXT("Body should fall down"), EndPos.Z < StartPos.Z);
    }
    
    // Test 5: Body should rotate (fall over)
    if (DeathAnimation.Num() >= 2)
    {
        FRotator StartRot = DeathAnimation[0].BoneTransforms[0].Rotator();
        FRotator EndRot = DeathAnimation[DeathAnimation.Num() - 1].BoneTransforms[0].Rotator();
        
        float RotationDiff = FMath::Abs(EndRot.Roll - StartRot.Roll);
        TestTrue(TEXT("Body should rotate during death"), RotationDiff > 30.0f);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest_GenerateAnimation, "EchoAlchemist.PCG.ProceduralAnimator.GenerateAnimation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorTest_GenerateAnimation::RunTest(const FString& Parameters)
{
    // Create a simple skeleton
    F2DSkeleton TestSkeleton;
    
    F2DBone RootBone;
    RootBone.BoneName = TEXT("Root");
    RootBone.ParentIndex = -1;
    RootBone.RestTransform = FTransform::Identity;
    TestSkeleton.Bones.Add(RootBone);

    // Test all animation types through the unified interface
    TArray<EAnimationType> AnimationTypes = {
        EAnimationType::Idle,
        EAnimationType::Walk,
        EAnimationType::Attack,
        EAnimationType::Death,
        EAnimationType::Jump,
        EAnimationType::Hit
    };

    for (EAnimationType AnimType : AnimationTypes)
    {
        TArray<FAnimationFrame> Animation = UProceduralAnimator::GenerateAnimation(TestSkeleton, AnimType, 1.0f);
        
        FString AnimTypeName = UEnum::GetValueAsString(AnimType);
        TestTrue(FString::Printf(TEXT("%s animation should have frames"), *AnimTypeName),
                Animation.Num() > 0);
        
        for (const FAnimationFrame& Frame : Animation)
        {
            TestEqual(FString::Printf(TEXT("%s: Each frame should have transforms"), *AnimTypeName),
                     Frame.BoneTransforms.Num(), TestSkeleton.Bones.Num());
        }
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest_DifferentDurations, "EchoAlchemist.PCG.ProceduralAnimator.DifferentDurations", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorTest_DifferentDurations::RunTest(const FString& Parameters)
{
    // Create a simple skeleton
    F2DSkeleton TestSkeleton;
    
    F2DBone RootBone;
    RootBone.BoneName = TEXT("Root");
    RootBone.ParentIndex = -1;
    RootBone.RestTransform = FTransform::Identity;
    TestSkeleton.Bones.Add(RootBone);

    // Test different durations
    TArray<FAnimationFrame> ShortAnim = UProceduralAnimator::GenerateIdleAnimation(TestSkeleton, 0.5f);
    TArray<FAnimationFrame> NormalAnim = UProceduralAnimator::GenerateIdleAnimation(TestSkeleton, 1.0f);
    TArray<FAnimationFrame> LongAnim = UProceduralAnimator::GenerateIdleAnimation(TestSkeleton, 2.0f);

    // All should have frames
    TestTrue(TEXT("Short animation should have frames"), ShortAnim.Num() > 0);
    TestTrue(TEXT("Normal animation should have frames"), NormalAnim.Num() > 0);
    TestTrue(TEXT("Long animation should have frames"), LongAnim.Num() > 0);

    // Frame times should reflect duration
    if (ShortAnim.Num() > 0 && LongAnim.Num() > 0)
    {
        float ShortLastTime = ShortAnim[ShortAnim.Num() - 1].Time;
        float LongLastTime = LongAnim[LongAnim.Num() - 1].Time;
        
        TestTrue(TEXT("Longer duration should have longer total time"),
                LongLastTime > ShortLastTime);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest_EmptySkeleton, "EchoAlchemist.PCG.ProceduralAnimator.EmptySkeleton", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorTest_EmptySkeleton::RunTest(const FString& Parameters)
{
    // Test with empty skeleton
    F2DSkeleton EmptySkeleton;

    TArray<FAnimationFrame> IdleAnim = UProceduralAnimator::GenerateIdleAnimation(EmptySkeleton, 1.0f);
    TArray<FAnimationFrame> WalkAnim = UProceduralAnimator::GenerateWalkAnimation(EmptySkeleton, 1.0f);
    TArray<FAnimationFrame> AttackAnim = UProceduralAnimator::GenerateAttackAnimation(EmptySkeleton, FVector(1, 0, 0), 1.0f);

    // Should handle empty skeleton gracefully (either return empty or minimal frames)
    TestTrue(TEXT("Empty skeleton should not crash idle animation"), true);
    TestTrue(TEXT("Empty skeleton should not crash walk animation"), true);
    TestTrue(TEXT("Empty skeleton should not crash attack animation"), true);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorTest_ComplexSkeleton, "EchoAlchemist.PCG.ProceduralAnimator.ComplexSkeleton", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorTest_ComplexSkeleton::RunTest(const FString& Parameters)
{
    // Create a complex skeleton with multiple bones
    F2DSkeleton ComplexSkeleton;
    
    // Root
    F2DBone Root;
    Root.BoneName = TEXT("Root");
    Root.ParentIndex = -1;
    Root.RestTransform = FTransform::Identity;
    ComplexSkeleton.Bones.Add(Root);
    
    // Body
    F2DBone Body;
    Body.BoneName = TEXT("Body");
    Body.ParentIndex = 0;
    Body.RestTransform = FTransform(FVector(0, 0, 50));
    ComplexSkeleton.Bones.Add(Body);
    
    // Head
    F2DBone Head;
    Head.BoneName = TEXT("Head");
    Head.ParentIndex = 1;
    Head.RestTransform = FTransform(FVector(0, 0, 30));
    ComplexSkeleton.Bones.Add(Head);
    
    // Legs
    F2DBone LeftLeg;
    LeftLeg.BoneName = TEXT("LeftLeg");
    LeftLeg.ParentIndex = 0;
    LeftLeg.RestTransform = FTransform(FVector(-15, 0, 0));
    ComplexSkeleton.Bones.Add(LeftLeg);
    
    F2DBone RightLeg;
    RightLeg.BoneName = TEXT("RightLeg");
    RightLeg.ParentIndex = 0;
    RightLeg.RestTransform = FTransform(FVector(15, 0, 0));
    ComplexSkeleton.Bones.Add(RightLeg);
    
    // Arms
    F2DBone LeftArm;
    LeftArm.BoneName = TEXT("LeftArm");
    LeftArm.ParentIndex = 1;
    LeftArm.RestTransform = FTransform(FVector(-25, 0, 40));
    ComplexSkeleton.Bones.Add(LeftArm);
    
    F2DBone RightArm;
    RightArm.BoneName = TEXT("RightArm");
    RightArm.ParentIndex = 1;
    RightArm.RestTransform = FTransform(FVector(25, 0, 40));
    ComplexSkeleton.Bones.Add(RightArm);

    // Generate all animation types
    TArray<FAnimationFrame> IdleAnim = UProceduralAnimator::GenerateIdleAnimation(ComplexSkeleton, 1.0f);
    TArray<FAnimationFrame> WalkAnim = UProceduralAnimator::GenerateWalkAnimation(ComplexSkeleton, 1.0f);
    TArray<FAnimationFrame> AttackAnim = UProceduralAnimator::GenerateAttackAnimation(ComplexSkeleton, FVector(1, 0, 0), 1.0f);
    TArray<FAnimationFrame> DeathAnim = UProceduralAnimator::GenerateDeathAnimation(ComplexSkeleton, FVector(1, 0, 0));

    // All animations should work with complex skeleton
    TestTrue(TEXT("Complex skeleton idle animation should have frames"), IdleAnim.Num() > 0);
    TestTrue(TEXT("Complex skeleton walk animation should have frames"), WalkAnim.Num() > 0);
    TestTrue(TEXT("Complex skeleton attack animation should have frames"), AttackAnim.Num() > 0);
    TestTrue(TEXT("Complex skeleton death animation should have frames"), DeathAnim.Num() > 0);

    // All frames should have transforms for all bones
    for (const FAnimationFrame& Frame : WalkAnim)
    {
        TestEqual(TEXT("Walk animation should have transforms for all bones"),
                 Frame.BoneTransforms.Num(), ComplexSkeleton.Bones.Num());
    }

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
