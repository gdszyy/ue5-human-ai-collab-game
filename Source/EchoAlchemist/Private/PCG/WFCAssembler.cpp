
#include "PCG/WFCAssembler.h"

// Helper struct for the WFC process
struct FWFC_Cell
{
    TArray<int32> PossibleModuleIndices;
    bool bIsCollapsed = false;
};

// Helper struct for backtracking
struct FWFC_State
{
    TArray<FWFC_Cell> Grid;
    int32 CellToCollapseIndex;
    int32 ChosenModuleIndex;
};

// Helper function to get connectors for a specific direction
static TArray<FWFCConnector> GetConnectorsForDirection(const FWFCModule& Module, FName Direction)
{
    TArray<FWFCConnector> Result;
    for (const FWFCConnector& Connector : Module.Connectors)
    {
        if (Connector.Direction == Direction)
        {
            Result.Add(Connector);
        }
    }
    return Result;
}

// Helper function to check if two connector sets are compatible
static bool AreConnectorSetsCompatible(const TArray<FWFCConnector>& Set1, const TArray<FWFCConnector>& Set2)
{
    // Two connector sets are compatible if they have at least one matching connector type
    for (const FWFCConnector& Connector1 : Set1)
    {
        for (const FWFCConnector& Connector2 : Set2)
        {
            if (Connector1.ConnectorType == Connector2.ConnectorType)
            {
                return true;
            }
        }
    }
    return false;
}

// Helper function to get neighbor cell index
static int32 GetNeighborIndex(int32 CellIndex, int32 Width, int32 Height, FName Direction)
{
    int32 X = CellIndex % Width;
    int32 Y = CellIndex / Width;
    
    if (Direction == TEXT("North"))
    {
        Y--;
    }
    else if (Direction == TEXT("South"))
    {
        Y++;
    }
    else if (Direction == TEXT("East"))
    {
        X++;
    }
    else if (Direction == TEXT("West"))
    {
        X--;
    }
    
    // Check bounds
    if (X < 0 || X >= Width || Y < 0 || Y >= Height)
    {
        return -1;
    }
    
    return Y * Width + X;
}

// Helper function to get opposite direction
static FName GetOppositeDirection(FName Direction)
{
    if (Direction == TEXT("North")) return TEXT("South");
    if (Direction == TEXT("South")) return TEXT("North");
    if (Direction == TEXT("East")) return TEXT("West");
    if (Direction == TEXT("West")) return TEXT("East");
    return NAME_None;
}

FWFCAssembly UWFCAssembler::AssembleWithWFC(const TArray<FWFCModule>& Modules, int32 Width, int32 Height, int32 Seed)
{
    FWFCAssembly Assembly;
    Assembly.Width = Width;
    Assembly.Height = Height;
    Assembly.PlacedModules.Init(NAME_None, Width * Height);

    if (Modules.Num() == 0) return Assembly;

    TArray<FWFC_Cell> Grid;
    Grid.Init(FWFC_Cell(), Width * Height);
    TArray<int32> AllModuleIndices;
    for(int32 i = 0; i < Modules.Num(); ++i) { AllModuleIndices.Add(i); }
    for(FWFC_Cell& Cell : Grid) { Cell.PossibleModuleIndices = AllModuleIndices; }

    FRandomStream RandStream(Seed);
    TArray<FWFC_State> StateStack;

    int32 CollapsedCells = 0;
    int32 MaxIterations = Width * Height * 100; // Prevent infinite loops
    int32 Iterations = 0;
    
    while (CollapsedCells < Width * Height && Iterations < MaxIterations)
    {
        Iterations++;
        
        int32 LowestEntropy = Modules.Num() + 1;
        TArray<int32> LowestEntropyCells;
        for (int32 i = 0; i < Grid.Num(); ++i)
        {
            if (!Grid[i].bIsCollapsed)
            {
                int32 CurrentEntropy = Grid[i].PossibleModuleIndices.Num();
                if (CurrentEntropy == 0)
                {
                    // Contradiction detected
                    continue;
                }
                if (CurrentEntropy < LowestEntropy)
                {
                    LowestEntropy = CurrentEntropy;
                    LowestEntropyCells.Empty();
                    LowestEntropyCells.Add(i);
                }
                else if (CurrentEntropy == LowestEntropy)
                {
                    LowestEntropyCells.Add(i);
                }
            }
        }

        if (LowestEntropyCells.Num() == 0) break;

        int32 CellToCollapseIndex = LowestEntropyCells[RandStream.RandRange(0, LowestEntropyCells.Num() - 1)];
        FWFC_Cell& CellToCollapse = Grid[CellToCollapseIndex];
        
        if (CellToCollapse.PossibleModuleIndices.Num() == 0)
        {
            // Backtrack
            if (StateStack.Num() > 0)
            {
                FWFC_State LastState = StateStack.Pop();
                Grid = LastState.Grid;
                Grid[LastState.CellToCollapseIndex].PossibleModuleIndices.Remove(LastState.ChosenModuleIndex);
                CollapsedCells--;
                continue;
            }
            else
            {
                // Cannot backtrack further, assembly failed
                break;
            }
        }
        
        int32 ChosenModuleIndex = CellToCollapse.PossibleModuleIndices[RandStream.RandRange(0, CellToCollapse.PossibleModuleIndices.Num() - 1)];

        // Save state for backtracking
        FWFC_State CurrentState;
        CurrentState.Grid = Grid;
        CurrentState.CellToCollapseIndex = CellToCollapseIndex;
        CurrentState.ChosenModuleIndex = ChosenModuleIndex;
        StateStack.Push(CurrentState);

        CellToCollapse.PossibleModuleIndices.Empty();
        CellToCollapse.PossibleModuleIndices.Add(ChosenModuleIndex);
        CellToCollapse.bIsCollapsed = true;
        CollapsedCells++;

        // Propagate constraints
        TArray<int32> PropagationStack;
        PropagationStack.Add(CellToCollapseIndex);

        bool bContradiction = false;
        while (PropagationStack.Num() > 0)
        {
            int32 CurrentCellIndex = PropagationStack.Pop();
            
            // Check all four directions
            TArray<FName> Directions = { TEXT("North"), TEXT("South"), TEXT("East"), TEXT("West") };
            
            for (FName Direction : Directions)
            {
                int32 NeighborIndex = GetNeighborIndex(CurrentCellIndex, Width, Height, Direction);
                if (NeighborIndex == -1) continue; // Out of bounds
                
                FWFC_Cell& NeighborCell = Grid[NeighborIndex];
                if (NeighborCell.bIsCollapsed) continue; // Already collapsed
                
                // Get the connectors of the current cell in this direction
                TArray<FWFCConnector> CurrentCellConnectors;
                for (int32 ModuleIndex : Grid[CurrentCellIndex].PossibleModuleIndices)
                {
                    TArray<FWFCConnector> ModuleConnectors = GetConnectorsForDirection(Modules[ModuleIndex], Direction);
                    CurrentCellConnectors.Append(ModuleConnectors);
                }
                
                // Filter neighbor's possible modules based on compatibility
                TArray<int32> NewPossibleModules;
                FName OppositeDir = GetOppositeDirection(Direction);
                
                for (int32 NeighborModuleIndex : NeighborCell.PossibleModuleIndices)
                {
                    TArray<FWFCConnector> NeighborConnectors = GetConnectorsForDirection(Modules[NeighborModuleIndex], OppositeDir);
                    
                    // Check if this neighbor module is compatible with any of the current cell's modules
                    bool bIsCompatible = false;
                    if (CurrentCellConnectors.Num() == 0 || NeighborConnectors.Num() == 0)
                    {
                        // If either has no connectors in this direction, assume compatible
                        bIsCompatible = true;
                    }
                    else
                    {
                        bIsCompatible = AreConnectorSetsCompatible(CurrentCellConnectors, NeighborConnectors);
                    }
                    
                    if (bIsCompatible)
                    {
                        NewPossibleModules.Add(NeighborModuleIndex);
                    }
                }
                
                // Update neighbor cell if possibilities changed
                if (NewPossibleModules.Num() < NeighborCell.PossibleModuleIndices.Num())
                {
                    NeighborCell.PossibleModuleIndices = NewPossibleModules;
                    
                    if (NewPossibleModules.Num() == 0)
                    {
                        // Contradiction detected
                        bContradiction = true;
                        break;
                    }
                    
                    // Add neighbor to propagation stack
                    PropagationStack.Add(NeighborIndex);
                }
            }
            
            if (bContradiction) break;
        }

        if (bContradiction)
        {
            if (StateStack.Num() > 0)
            {
                FWFC_State LastState = StateStack.Pop();
                Grid = LastState.Grid;
                Grid[LastState.CellToCollapseIndex].PossibleModuleIndices.Remove(LastState.ChosenModuleIndex);
                CollapsedCells--;
            }
            else
            {
                // Cannot backtrack further, assembly failed
                break;
            }
        }
    }

    // Fill in the final assembly
    for (int32 i = 0; i < Grid.Num(); ++i)
    {
        if (Grid[i].bIsCollapsed && Grid[i].PossibleModuleIndices.Num() > 0)
        {
            Assembly.PlacedModules[i] = Modules[Grid[i].PossibleModuleIndices[0]].ModuleId;
        }
    }

    return Assembly;
}
