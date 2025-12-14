
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralAnimator.generated.h"

/**
 * @enum EAnimationType
 * @brief Defines the type of animation to generate.
 */
UENUM(BlueprintType)
enum class EAnimationType : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Walk UMETA(DisplayName = "Walk"),
    Attack UMETA(DisplayName = "Attack"),
    Death UMETA(DisplayName = "Death"),
    Jump UMETA(DisplayName = "Jump"),
    Hit UMETA(DisplayName = "Hit")
};

/**
 * @struct F2DBone
 * @brief Defines a single bone in a 2D skeleton.
 */
USTRUCT(BlueprintType)
struct F2DBone
{
    GENERATED_BODY()

    /** The name of this bone. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    FName BoneName;

    /** The name of the parent bone. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    FName ParentName;
    
    /** The index of the parent bone (-1 for root). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    int32 ParentIndex = -1;

    /** The transform of this bone relative to its parent. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    FTransform Transform;
    
    /** The rest pose transform (used for procedural animation). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    FTransform RestTransform;
};

/**
 * @struct F2DSkeleton
 * @brief Represents a 2D skeleton.
 */
USTRUCT(BlueprintType)
struct F2DSkeleton
{
    GENERATED_BODY()

    /** The bones in this skeleton. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TArray<F2DBone> Bones;
};

/**
 * @struct FAnimationFrame
 * @brief Represents a single frame of animation.
 */
USTRUCT(BlueprintType)
struct FAnimationFrame
{
    GENERATED_BODY()

    /** The transforms for all bones in this frame. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TArray<FTransform> BoneTransforms;
    
    /** The time of this frame in the animation (0-1). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float Time;
};

/**
 * @class UProceduralAnimator
 * @brief A Blueprint Function Library for generating procedural 2D animations.
 */
UCLASS()
class ECHOALCHEMIST_API UProceduralAnimator : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
     * Generates a procedural walk animation for a 2D skeleton.
     *
     * @param Skeleton The skeleton to animate.
     * @param Speed The speed of the walk cycle.
     * @param Amplitude The amplitude of the bone movements.
     * @return An array of animation frames representing the walk animation.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Animation", meta = (DisplayName = "Generate Walk Animation", Keywords = "animation procedural walk skeleton"))
    static TArray<FAnimationFrame> GenerateWalkAnimation(const F2DSkeleton& Skeleton, float Speed = 1.0f, float Amplitude = 10.0f);

    /**
     * Generates a procedural idle animation for a 2D skeleton.
     *
     * @param Skeleton The skeleton to animate.
     * @param Speed The speed of the breathing cycle.
     * @param Amplitude The amplitude of the breathing movement.
     * @return An array of animation frames representing the idle animation.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Animation", meta = (DisplayName = "Generate Idle Animation", Keywords = "animation procedural idle skeleton"))
    static TArray<FAnimationFrame> GenerateIdleAnimation(const F2DSkeleton& Skeleton, float Speed = 0.5f, float Amplitude = 5.0f);

    /**
     * Generates a procedural attack animation for a 2D skeleton.
     *
     * @param Skeleton The skeleton to animate.
     * @param AttackDirection The direction of the attack (normalized vector).
     * @param AttackSpeed The speed of the attack animation.
     * @return An array of animation frames representing the attack animation.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Animation", meta = (DisplayName = "Generate Attack Animation", Keywords = "animation procedural attack skeleton"))
    static TArray<FAnimationFrame> GenerateAttackAnimation(const F2DSkeleton& Skeleton, FVector AttackDirection = FVector(1, 0, 0), float AttackSpeed = 2.0f);

    /**
     * Generates a procedural death animation for a 2D skeleton.
     *
     * @param Skeleton The skeleton to animate.
     * @param FallDirection The direction the creature falls.
     * @return An array of animation frames representing the death animation.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Animation", meta = (DisplayName = "Generate Death Animation", Keywords = "animation procedural death skeleton"))
    static TArray<FAnimationFrame> GenerateDeathAnimation(const F2DSkeleton& Skeleton, FVector FallDirection = FVector(0, 0, -1));

    /**
     * Generates a procedural animation based on animation type.
     *
     * @param Skeleton The skeleton to animate.
     * @param AnimationType The type of animation to generate.
     * @param Speed The speed of the animation.
     * @param Amplitude The amplitude of the movements.
     * @return An array of animation frames.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Animation", meta = (DisplayName = "Generate Animation", Keywords = "animation procedural skeleton"))
    static TArray<FAnimationFrame> GenerateAnimation(const F2DSkeleton& Skeleton, EAnimationType AnimationType, float Speed = 1.0f, float Amplitude = 10.0f);

private:

    /** Helper function to apply a sine wave to a transform. */
    static FTransform ApplySineWave(const FTransform& BaseTransform, float Time, float Frequency, float Amplitude, FVector Axis);
    
    /** Helper function to interpolate between two transforms. */
    static FTransform InterpolateTransform(const FTransform& A, const FTransform& B, float Alpha);
};
