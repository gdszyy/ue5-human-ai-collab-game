#include "PCG/ShapeGenerator.h"

FShapeData UShapeGenerator::GenerateShapeWithCellularAutomata(int32 Width, int32 Height, int32 Seed, int32 Iterations, int32 BirthThreshold, int32 SurvivalThreshold)
{
    FShapeData Shape;
    Shape.Width = Width;
    Shape.Height = Height;
    Shape.Grid.Init(false, Width * Height);

    // 1. Random Initialization
    FRandomStream RandStream(Seed);
    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            Shape.Grid[y * Width + x] = RandStream.FRand() < 0.5f;
        }
    }

    // 2. Simulation
    for (int32 i = 0; i < Iterations; ++i)
    {
        TArray<bool> NextGrid = Shape.Grid;
        for (int32 y = 0; y < Height; ++y)
        {
            for (int32 x = 0; x < Width; ++x)
            {
                int32 Neighbors = 0;
                for (int32 ny = -1; ny <= 1; ++ny)
                {
                    for (int32 nx = -1; nx <= 1; ++nx)
                    {
                        if (nx == 0 && ny == 0) continue;
                        int32 CheckX = x + nx;
                        int32 CheckY = y + ny;
                        if (CheckX >= 0 && CheckX < Width && CheckY >= 0 && CheckY < Height && Shape.Grid[CheckY * Width + CheckX])
                        {
                            Neighbors++;
                        }
                    }
                }

                int32 CurrentIndex = y * Width + x;
                if (Shape.Grid[CurrentIndex] && Neighbors < SurvivalThreshold)
                {
                    NextGrid[CurrentIndex] = false;
                }
                else if (!Shape.Grid[CurrentIndex] && Neighbors > BirthThreshold)
                {
                    NextGrid[CurrentIndex] = true;
                }
            }
        }
        Shape.Grid = NextGrid;
    }

    return Shape;
}

FShapeData UShapeGenerator::GenerateShapeWithSimplexNoise(int32 Width, int32 Height, int32 Seed, float Scale, float Threshold)
{
    FShapeData Shape;
    Shape.Width = Width;
    Shape.Height = Height;
    Shape.Grid.Init(false, Width * Height);

    FRandomStream RandStream(Seed);

    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            // Use UE4's built-in Perlin noise instead of SimplexNoise
            float NoiseValue = FMath::PerlinNoise2D(FVector2D(x / Scale, y / Scale));
            Shape.Grid[y * Width + x] = NoiseValue > Threshold;
        }
    }

    return Shape;
}

FShapeData UShapeGenerator::GenerateShapeWithVoronoi(int32 Width, int32 Height, int32 Seed, int32 NumPoints)
{
    FShapeData Shape;
    Shape.Width = Width;
    Shape.Height = Height;
    Shape.Grid.Init(false, Width * Height);

    if (NumPoints <= 0) return Shape;

    FRandomStream RandStream(Seed);
    TArray<FVector2D> Points;
    for (int32 i = 0; i < NumPoints; ++i)
    {
        Points.Add(FVector2D(RandStream.FRandRange(0, Width), RandStream.FRandRange(0, Height)));
    }

    for (int32 y = 0; y < Height; ++y)
    {
        for (int32 x = 0; x < Width; ++x)
        {
            float MinDistance = -1.0f;
            int32 ClosestPointIndex = -1;

            for (int32 i = 0; i < Points.Num(); ++i)
            {
                float Dist = FVector2D::DistSquared(FVector2D(x, y), Points[i]);
                if (ClosestPointIndex == -1 || Dist < MinDistance)
                {
                    MinDistance = Dist;
                    ClosestPointIndex = i;
                }
            }
            // For simplicity, we fill the region of the first point
            if (ClosestPointIndex == 0)
            {
                 Shape.Grid[y * Width + x] = true;
            }
        }
    }

    return Shape;
}

FShapeData UShapeGenerator::GenerateShapeWithCrystalGrowth(int32 Width, int32 Height, int32 Seed, int32 Iterations, float GrowthChance)
{
    FShapeData Shape;
    Shape.Width = Width;
    Shape.Height = Height;
    Shape.Grid.Init(false, Width * Height);

    FRandomStream RandStream(Seed);

    // Start with a single seed in the center
    int32 CenterX = Width / 2;
    int32 CenterY = Height / 2;
    Shape.Grid[CenterY * Width + CenterX] = true;

    for (int32 i = 0; i < Iterations; ++i)
    {
        TArray<bool> NextGrid = Shape.Grid;
        for (int32 y = 0; y < Height; ++y)
        {
            for (int32 x = 0; x < Width; ++x)
            {
                if (Shape.Grid[y * Width + x]) continue; // Already grown

                int32 Neighbors = 0;
                for (int32 ny = -1; ny <= 1; ++ny)
                {
                    for (int32 nx = -1; nx <= 1; ++nx)
                    {
                        if (abs(nx) == abs(ny)) continue; // Only check orthogonal neighbors
                        int32 CheckX = x + nx;
                        int32 CheckY = y + ny;
                        if (CheckX >= 0 && CheckX < Width && CheckY >= 0 && CheckY < Height && Shape.Grid[CheckY * Width + CheckX])
                        {
                            Neighbors++;
                        }
                    }
                }

                if (Neighbors > 0 && RandStream.FRand() < GrowthChance)
                {
                    NextGrid[y * Width + x] = true;
                }
            }
        }
        Shape.Grid = NextGrid;
    }

    return Shape;
}
