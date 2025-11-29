
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralAnimator.generated.h"

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

    /** The transform of this bone relative to its parent. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    FTransform Transform;
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
     * @return An array of transforms representing the animation frames.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Animation", meta = (DisplayName = "Generate Walk Animation", Keywords = "animation procedural walk skeleton"))
    static TArray<FTransform> GenerateWalkAnimation(const F2DSkeleton& Skeleton, float Speed = 1.0f, float Amplitude = 10.0f);
};
