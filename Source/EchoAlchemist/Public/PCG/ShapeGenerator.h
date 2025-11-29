#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShapeGenerator.generated.h"

/**
 * @struct FShapeData
 * @brief Represents a 2D shape as a grid of boolean values.
 */
USTRUCT(BlueprintType)
struct FShapeData
{
    GENERATED_BODY()

    /** The 2D grid representing the shape. True for solid, false for empty. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shape")
    TArray<bool> Grid;

    /** The width of the shape grid. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shape")
    int32 Width;

    /** The height of the shape grid. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shape")
    int32 Height;
};

/**
 * @class UShapeGenerator
 * @brief A Blueprint Function Library for generating 2D shapes procedurally.
 */
UCLASS()
class ECHOALCHEMIST_API UShapeGenerator : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
     * Generates a 2D shape using a Cellular Automata algorithm (e.g., Conway's Game of Life).
     *
     * @param Width The width of the shape grid.
     * @param Height The height of the shape grid.
     * @param Seed A random seed for initialization.
     * @param Iterations The number of simulation steps to run.
     * @param BirthThreshold The number of neighbors required for a dead cell to become alive.
     * @param SurvivalThreshold The number of neighbors required for a live cell to survive.
     * @return A new FShapeData struct containing the generated shape.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Shape", meta = (DisplayName = "Generate Shape with Cellular Automata", Keywords = "shape generator cellular automata game of life"))
    static FShapeData GenerateShapeWithCellularAutomata(int32 Width, int32 Height, int32 Seed, int32 Iterations = 5, int32 BirthThreshold = 4, int32 SurvivalThreshold = 4);

    /**
     * Generates a 2D shape using Simplex Noise.
     *
     * @param Width The width of the shape grid.
     * @param Height The height of the shape grid.
     * @param Seed A random seed for the noise.
     * @param Scale The scale of the noise pattern.
     * @param Threshold The cutoff value to determine if a cell is solid or empty.
     * @return A new FShapeData struct containing the generated shape.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Shape", meta = (DisplayName = "Generate Shape with Simplex Noise", Keywords = "shape generator simplex noise perlin"))
    static FShapeData GenerateShapeWithSimplexNoise(int32 Width, int32 Height, int32 Seed, float Scale = 20.0f, float Threshold = 0.5f);
};

    /**
     * Generates a 2D shape using a Voronoi diagram.
     *
     * @param Width The width of the shape grid.
     * @param Height The height of the shape grid.
     * @param Seed A random seed for point placement.
     * @param NumPoints The number of points to use for the Voronoi diagram.
     * @return A new FShapeData struct containing the generated shape.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Shape", meta = (DisplayName = "Generate Shape with Voronoi", Keywords = "shape generator voronoi crystal"))
    static FShapeData GenerateShapeWithVoronoi(int32 Width, int32 Height, int32 Seed, int32 NumPoints = 10);

    /**
     * Generates a 2D shape using a crystal growth simulation.
     *
     * @param Width The width of the shape grid.
     * @param Height The height of the shape grid.
     * @param Seed A random seed for the growth process.
     * @param Iterations The number of growth iterations.
     * @param GrowthChance The probability of a new crystal forming.
     * @return A new FShapeData struct containing the generated shape.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Shape", meta = (DisplayName = "Generate Shape with Crystal Growth", Keywords = "shape generator crystal growth"))
    static FShapeData GenerateShapeWithCrystalGrowth(int32 Width, int32 Height, int32 Seed, int32 Iterations = 5, float GrowthChance = 0.3f);
