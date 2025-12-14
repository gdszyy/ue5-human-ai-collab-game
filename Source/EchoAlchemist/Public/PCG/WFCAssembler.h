
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WFCAssembler.generated.h"

/**
 * @struct FWFCConnector
 * @brief Defines a connection point on a module.
 */
USTRUCT(BlueprintType)
struct FWFCConnector
{
    GENERATED_BODY()

    /** The type of the connector (e.g., "head_socket", "torso_socket_top"). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    FName ConnectorType;

    /** The direction of the connector (e.g., North, South, East, West). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    FName Direction;
};

/**
 * @struct FWFCModule
 * @brief Represents a single module (part) in the WFC system.
 */
USTRUCT(BlueprintType)
struct FWFCModule
{
    GENERATED_BODY()

    /** A unique ID for this module. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    FName ModuleId;

    /** The connectors on this module. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    TArray<FWFCConnector> Connectors;
    
    /** Module ID (alternative naming). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    FName ModuleID;
    
    /** North connectors. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    TArray<FName> NorthConnectors;
    
    /** South connectors. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    TArray<FName> SouthConnectors;
    
    /** East connectors. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    TArray<FName> EastConnectors;
    
    /** West connectors. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    TArray<FName> WestConnectors;
};

/**
 * @struct FWFCCell
 * @brief Represents a single cell in the WFC grid.
 */
USTRUCT(BlueprintType)
struct FWFCCell
{
    GENERATED_BODY()
    
    /** The module ID placed in this cell. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    FName ModuleID;
    
    /** The position in the grid. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    FIntPoint Position;
};

/**
 * @struct FWFCAssembly
 * @brief Represents the final assembled creature.
 */
USTRUCT(BlueprintType)
struct FWFCAssembly
{
    GENERATED_BODY()

    /** The placed modules in a 2D grid. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    TArray<FName> PlacedModules;
    
    /** The grid of cells. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    TArray<FWFCCell> Grid;
    
    /** Whether the assembly was successful. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    bool bSuccess = false;

    /** The width of the assembly grid. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    int32 Width;

    /** The height of the assembly grid. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
    int32 Height;
};

/**
 * @class UWFCAssembler
 * @brief A Blueprint Function Library for assembling modules using Wave Function Collapse.
 */
UCLASS()
class ECHOALCHEMIST_API UWFCAssembler : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
     * Assembles a creature from a set of modules using Wave Function Collapse.
     *
     * @param Modules The library of modules to use.
     * @param Width The width of the assembly grid.
     * @param Height The height of the assembly grid.
     * @param Seed A random seed for the assembly process.
     * @return A new FWFCAssembly struct containing the assembled creature.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | WFC", meta = (DisplayName = "Assemble with WFC", Keywords = "wfc wave function collapse assemble creature"))
    static FWFCAssembly AssembleWithWFC(const TArray<FWFCModule>& Modules, int32 Width, int32 Height, int32 Seed);
};
