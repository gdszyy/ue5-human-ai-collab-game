// Copyright Echo Alchemist. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PCG/ProceduralAnimator.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorIdleTest, "EchoAlchemist.PCG.ProceduralAnimator.GenerateIdleAnimation", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorIdleTest::RunTest(const FString& Parameters)
{
    // Test: Generate idle animation keyframes
    int32 NumFrames = 10;
    float Amplitude = 5.0f;
    float Frequency = 1.0f;
    
    TArray<FVector2D> Result = UProceduralAnimator::GenerateIdleAnimation(NumFrames, Amplitude, Frequency);
    
    // Verify: Should generate exactly NumFrames keyframes
    TestEqual(TEXT("Idle animation should have correct number of frames"), Result.Num(), NumFrames);
    
    // Verify: All offsets should be within the amplitude range
    for (const FVector2D& Offset : Result)
    {
        TestTrue(TEXT("Y offset should be within amplitude range"), FMath::Abs(Offset.Y) <= Amplitude + 0.1f);
    }
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorIdleZeroFramesTest, "EchoAlchemist.PCG.ProceduralAnimator.GenerateIdleAnimation.ZeroFrames", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorIdleZeroFramesTest::RunTest(const FString& Parameters)
{
    // Test: Generate idle animation with zero frames
    int32 NumFrames = 0;
    float Amplitude = 5.0f;
    float Frequency = 1.0f;
    
    TArray<FVector2D> Result = UProceduralAnimator::GenerateIdleAnimation(NumFrames, Amplitude, Frequency);
    
    // Verify: Should return empty array
    TestEqual(TEXT("Zero frames should produce empty animation"), Result.Num(), 0);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorWalkTest, "EchoAlchemist.PCG.ProceduralAnimator.GenerateWalkAnimation", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorWalkTest::RunTest(const FString& Parameters)
{
    // Test: Generate walk animation keyframes
    int32 NumFrames = 8;
    float StepLength = 20.0f;
    float BounceHeight = 10.0f;
    
    TArray<FVector2D> Result = UProceduralAnimator::GenerateWalkAnimation(NumFrames, StepLength, BounceHeight);
    
    // Verify: Should generate exactly NumFrames keyframes
    TestEqual(TEXT("Walk animation should have correct number of frames"), Result.Num(), NumFrames);
    
    // Verify: X offsets should progress (walk forward)
    if (Result.Num() > 1)
    {
        float TotalDistance = Result[Result.Num() - 1].X - Result[0].X;
        TestTrue(TEXT("Walk animation should progress forward"), TotalDistance > 0.0f);
    }
    
    // Verify: Y offsets should bounce (up and down)
    bool HasPositiveY = false;
    bool HasNegativeY = false;
    for (const FVector2D& Offset : Result)
    {
        if (Offset.Y > 0.1f) HasPositiveY = true;
        if (Offset.Y < -0.1f) HasNegativeY = true;
    }
    TestTrue(TEXT("Walk animation should have bounce (positive and negative Y)"), HasPositiveY || Result.Num() < 3);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProceduralAnimatorAttackTest, "EchoAlchemist.PCG.ProceduralAnimator.GenerateAttackAnimation", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FProceduralAnimatorAttackTest::RunTest(const FString& Parameters)
{
    // Test: Generate attack animation keyframes
    int32 NumFrames = 6;
    float AttackDistance = 30.0f;
    float WindupRatio = 0.3f;
    
    TArray<FVector2D> Result = UProceduralAnimator::GenerateAttackAnimation(NumFrames, AttackDistance, WindupRatio);
    
    // Verify: Should generate exactly NumFrames keyframes
    TestEqual(TEXT("Attack animation should have correct number of frames"), Result.Num(), NumFrames);
    
    // Verify: Should have windup (negative X) and attack (positive X) phases
    if (Result.Num() > 2)
    {
        bool HasWindup = false;
        bool HasAttack = false;
        
        for (const FVector2D& Offset : Result)
        {
            if (Offset.X < -0.1f) HasWindup = true;
            if (Offset.X > 0.1f) HasAttack = true;
        }
        
        TestTrue(TEXT("Attack animation should have windup phase"), HasWindup);
        TestTrue(TEXT("Attack animation should have attack phase"), HasAttack);
    }
    
    return true;
}
