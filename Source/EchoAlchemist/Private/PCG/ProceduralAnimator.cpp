
#include "PCG/ProceduralAnimator.h"

FTransform UProceduralAnimator::ApplySineWave(const FTransform& BaseTransform, float Time, float Frequency, float Amplitude, FVector Axis)
{
    FTransform Result = BaseTransform;
    float Sine = FMath::Sin(Time * 2.0f * PI * Frequency);
    FVector Offset = Axis * Sine * Amplitude;
    Result.AddToTranslation(Offset);
    return Result;
}

FTransform UProceduralAnimator::InterpolateTransform(const FTransform& A, const FTransform& B, float Alpha)
{
    FTransform Result;
    Result.SetLocation(FMath::Lerp(A.GetLocation(), B.GetLocation(), Alpha));
    Result.SetRotation(FQuat::Slerp(A.GetRotation(), B.GetRotation(), Alpha));
    Result.SetScale3D(FMath::Lerp(A.GetScale3D(), B.GetScale3D(), Alpha));
    return Result;
}

TArray<FAnimationFrame> UProceduralAnimator::GenerateWalkAnimation(const F2DSkeleton& Skeleton, float Speed, float Amplitude)
{
    TArray<FAnimationFrame> AnimationFrames;

    if (Skeleton.Bones.Num() == 0) return AnimationFrames;

    const int32 NumFrames = 30;
    
    for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
    {
        FAnimationFrame Frame;
        Frame.Time = (float)FrameIndex / (float)NumFrames;
        
        for (int32 BoneIndex = 0; BoneIndex < Skeleton.Bones.Num(); ++BoneIndex)
        {
            const F2DBone& Bone = Skeleton.Bones[BoneIndex];
            FTransform NewTransform = Bone.Transform;
            
            // Different bones move differently in a walk cycle
            FName BoneName = Bone.BoneName;
            
            if (BoneName.ToString().Contains(TEXT("Leg")) || BoneName.ToString().Contains(TEXT("Foot")))
            {
                // Legs move forward and backward
                float LegPhase = Frame.Time * Speed;
                // Alternate legs (odd vs even index)
                if (BoneIndex % 2 == 0)
                {
                    LegPhase += 0.5f;
                }
                float LegSine = FMath::Sin(LegPhase * 2.0f * PI);
                NewTransform.AddToTranslation(FVector(LegSine * Amplitude, 0, 0));
            }
            else if (BoneName.ToString().Contains(TEXT("Body")) || BoneName.ToString().Contains(TEXT("Torso")))
            {
                // Body bobs up and down
                float BodySine = FMath::Sin(Frame.Time * 2.0f * PI * Speed * 2.0f);
                NewTransform.AddToTranslation(FVector(0, 0, FMath::Abs(BodySine) * Amplitude * 0.3f));
            }
            else if (BoneName.ToString().Contains(TEXT("Head")))
            {
                // Head bobs slightly
                float HeadSine = FMath::Sin(Frame.Time * 2.0f * PI * Speed);
                NewTransform.AddToTranslation(FVector(0, 0, HeadSine * Amplitude * 0.2f));
            }
            else if (BoneName.ToString().Contains(TEXT("Arm")) || BoneName.ToString().Contains(TEXT("Wing")))
            {
                // Arms/wings swing
                float ArmPhase = Frame.Time * Speed;
                if (BoneIndex % 2 == 0)
                {
                    ArmPhase += 0.5f;
                }
                float ArmSine = FMath::Sin(ArmPhase * 2.0f * PI);
                FRotator Rotation = NewTransform.GetRotation().Rotator();
                Rotation.Roll += ArmSine * 15.0f; // Swing arms
                NewTransform.SetRotation(Rotation.Quaternion());
            }
            else
            {
                // Default: slight vertical movement
                float DefaultSine = FMath::Sin(Frame.Time * 2.0f * PI * Speed);
                NewTransform.AddToTranslation(FVector(0, 0, DefaultSine * Amplitude * 0.1f));
            }
            
            Frame.BoneTransforms.Add(NewTransform);
        }
        
        AnimationFrames.Add(Frame);
    }

    return AnimationFrames;
}

TArray<FAnimationFrame> UProceduralAnimator::GenerateIdleAnimation(const F2DSkeleton& Skeleton, float Speed, float Amplitude)
{
    TArray<FAnimationFrame> AnimationFrames;

    if (Skeleton.Bones.Num() == 0) return AnimationFrames;

    const int32 NumFrames = 24;
    
    for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
    {
        FAnimationFrame Frame;
        Frame.Time = (float)FrameIndex / (float)NumFrames;
        
        for (int32 BoneIndex = 0; BoneIndex < Skeleton.Bones.Num(); ++BoneIndex)
        {
            const F2DBone& Bone = Skeleton.Bones[BoneIndex];
            FTransform NewTransform = Bone.Transform;
            
            FName BoneName = Bone.BoneName;
            
            // Breathing animation - gentle up and down movement
            float BreathSine = FMath::Sin(Frame.Time * 2.0f * PI * Speed);
            
            if (BoneName.ToString().Contains(TEXT("Body")) || BoneName.ToString().Contains(TEXT("Torso")))
            {
                // Body expands and contracts
                NewTransform.AddToTranslation(FVector(0, 0, BreathSine * Amplitude * 0.5f));
                FVector Scale = NewTransform.GetScale3D();
                Scale.Z += BreathSine * 0.05f;
                NewTransform.SetScale3D(Scale);
            }
            else if (BoneName.ToString().Contains(TEXT("Head")))
            {
                // Head moves slightly
                NewTransform.AddToTranslation(FVector(0, 0, BreathSine * Amplitude * 0.3f));
            }
            else
            {
                // Other bones move very slightly
                NewTransform.AddToTranslation(FVector(0, 0, BreathSine * Amplitude * 0.1f));
            }
            
            Frame.BoneTransforms.Add(NewTransform);
        }
        
        AnimationFrames.Add(Frame);
    }

    return AnimationFrames;
}

TArray<FAnimationFrame> UProceduralAnimator::GenerateAttackAnimation(const F2DSkeleton& Skeleton, FVector AttackDirection, float AttackSpeed)
{
    TArray<FAnimationFrame> AnimationFrames;

    if (Skeleton.Bones.Num() == 0) return AnimationFrames;

    AttackDirection.Normalize();
    const int32 NumFrames = 20;
    
    for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
    {
        FAnimationFrame Frame;
        Frame.Time = (float)FrameIndex / (float)NumFrames;
        
        // Attack animation has three phases: windup (0-0.3), strike (0.3-0.5), recovery (0.5-1.0)
        float WindupEnd = 0.3f;
        float StrikeEnd = 0.5f;
        
        for (int32 BoneIndex = 0; BoneIndex < Skeleton.Bones.Num(); ++BoneIndex)
        {
            const F2DBone& Bone = Skeleton.Bones[BoneIndex];
            FTransform NewTransform = Bone.Transform;
            
            FName BoneName = Bone.BoneName;
            
            if (BoneName.ToString().Contains(TEXT("Arm")) || BoneName.ToString().Contains(TEXT("Claw")) || 
                BoneName.ToString().Contains(TEXT("Wing")) || BoneName.ToString().Contains(TEXT("Weapon")))
            {
                // Attack limbs move forward
                if (Frame.Time < WindupEnd)
                {
                    // Windup - pull back
                    float WindupProgress = Frame.Time / WindupEnd;
                    NewTransform.AddToTranslation(-AttackDirection * 20.0f * WindupProgress);
                }
                else if (Frame.Time < StrikeEnd)
                {
                    // Strike - fast forward
                    float StrikeProgress = (Frame.Time - WindupEnd) / (StrikeEnd - WindupEnd);
                    NewTransform.AddToTranslation(AttackDirection * 50.0f * StrikeProgress - AttackDirection * 20.0f);
                }
                else
                {
                    // Recovery - return to position
                    float RecoveryProgress = (Frame.Time - StrikeEnd) / (1.0f - StrikeEnd);
                    NewTransform.AddToTranslation(AttackDirection * 30.0f * (1.0f - RecoveryProgress));
                }
            }
            else if (BoneName.ToString().Contains(TEXT("Body")) || BoneName.ToString().Contains(TEXT("Torso")))
            {
                // Body leans into the attack
                if (Frame.Time < StrikeEnd)
                {
                    float LeanProgress = Frame.Time / StrikeEnd;
                    NewTransform.AddToTranslation(AttackDirection * 10.0f * LeanProgress);
                }
                else
                {
                    float RecoveryProgress = (Frame.Time - StrikeEnd) / (1.0f - StrikeEnd);
                    NewTransform.AddToTranslation(AttackDirection * 10.0f * (1.0f - RecoveryProgress));
                }
            }
            
            Frame.BoneTransforms.Add(NewTransform);
        }
        
        AnimationFrames.Add(Frame);
    }

    return AnimationFrames;
}

TArray<FAnimationFrame> UProceduralAnimator::GenerateDeathAnimation(const F2DSkeleton& Skeleton, FVector FallDirection)
{
    TArray<FAnimationFrame> AnimationFrames;

    if (Skeleton.Bones.Num() == 0) return AnimationFrames;

    FallDirection.Normalize();
    const int32 NumFrames = 30;
    
    for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
    {
        FAnimationFrame Frame;
        Frame.Time = (float)FrameIndex / (float)NumFrames;
        
        // Death animation: creature collapses and falls
        float CollapseProgress = FMath::Pow(Frame.Time, 1.5f); // Ease in
        
        for (int32 BoneIndex = 0; BoneIndex < Skeleton.Bones.Num(); ++BoneIndex)
        {
            const F2DBone& Bone = Skeleton.Bones[BoneIndex];
            FTransform NewTransform = Bone.Transform;
            
            FName BoneName = Bone.BoneName;
            
            // All bones fall down
            NewTransform.AddToTranslation(FallDirection * 100.0f * CollapseProgress);
            
            // Add rotation for dramatic effect
            FRotator Rotation = NewTransform.GetRotation().Rotator();
            Rotation.Roll += CollapseProgress * 90.0f;
            NewTransform.SetRotation(Rotation.Quaternion());
            
            // Limbs go limp
            if (BoneName.ToString().Contains(TEXT("Leg")) || BoneName.ToString().Contains(TEXT("Arm")))
            {
                FRotator LimbRotation = NewTransform.GetRotation().Rotator();
                LimbRotation.Pitch += CollapseProgress * 45.0f;
                NewTransform.SetRotation(LimbRotation.Quaternion());
            }
            
            Frame.BoneTransforms.Add(NewTransform);
        }
        
        AnimationFrames.Add(Frame);
    }

    return AnimationFrames;
}

TArray<FAnimationFrame> UProceduralAnimator::GenerateAnimation(const F2DSkeleton& Skeleton, EAnimationType AnimationType, float Speed, float Amplitude)
{
    switch (AnimationType)
    {
    case EAnimationType::Idle:
        return GenerateIdleAnimation(Skeleton, Speed, Amplitude);
    case EAnimationType::Walk:
        return GenerateWalkAnimation(Skeleton, Speed, Amplitude);
    case EAnimationType::Attack:
        return GenerateAttackAnimation(Skeleton, FVector(1, 0, 0), Speed);
    case EAnimationType::Death:
        return GenerateDeathAnimation(Skeleton, FVector(0, 0, -1));
    case EAnimationType::Jump:
        // Jump animation can be implemented similarly
        return GenerateIdleAnimation(Skeleton, Speed * 2.0f, Amplitude * 2.0f);
    case EAnimationType::Hit:
        // Hit animation - quick recoil
        return GenerateAttackAnimation(Skeleton, FVector(-1, 0, 0), Speed * 2.0f);
    default:
        return GenerateIdleAnimation(Skeleton, Speed, Amplitude);
    }
}
