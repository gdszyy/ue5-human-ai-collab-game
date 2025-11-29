'''
#include "PCG/WFCAssembler.h"

// Helper struct for the WFC process
struct FWFC_Cell
{
    TArray<int32> PossibleModuleIndices; // Indices into the main Modules array
    bool bIsCollapsed = false;
};

// Helper function to check connector compatibility
bool AreConnectorsCompatible(const FWFCConnector& A, const FWFCConnector& B)
{
    // A simple compatibility rule: types must match, and directions must be opposite
    // e.g., North connects to South
    if (A.ConnectorType != B.ConnectorType) return false;

    if ((A.Direction == "North" && B.Direction == "South") ||
        (A.Direction == "South" && B.Direction == "North") ||
        (A.Direction == "East" && B.Direction == "West") ||
        (A.Direction == "West" && B.Direction == "East"))
    {
        return true;
    }

    return false;
}

FWFCAssembly UWFCAssembler::AssembleWithWFC(const TArray<FWFCModule>& Modules, int32 Width, int32 Height, int32 Seed)
{
    FWFCAssembly Assembly;
    Assembly.Width = Width;
    Assembly.Height = Height;
    Assembly.PlacedModules.Init(NAME_None, Width * Height);

    if (Modules.Num() == 0) return Assembly;

    // 1. Initialization
    TArray<FWFC_Cell> Grid;
    Grid.Init(FWFC_Cell(), Width * Height);
    TArray<int32> AllModuleIndices;
    for(int32 i = 0; i < Modules.Num(); ++i) { AllModuleIndices.Add(i); }
    for(FWFC_Cell& Cell : Grid) { Cell.PossibleModuleIndices = AllModuleIndices; }

    FRandomStream RandStream(Seed);
    TArray<int32> PropagationStack;

    // Main WFC Loop
    for(int k = 0; k < Width * Height; ++k)
    {
        // 2. Observation (Find lowest entropy cell)
        int32 LowestEntropy = Modules.Num() + 1;
        int32 CellToCollapseIndex = -1;
        TArray<int32> LowestEntropyCells;

        for (int32 i = 0; i < Grid.Num(); ++i)
        {
            if (!Grid[i].bIsCollapsed)
            {
                int32 CurrentEntropy = Grid[i].PossibleModuleIndices.Num();
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

        if (LowestEntropyCells.Num() == 0) break; // All cells collapsed

        CellToCollapseIndex = LowestEntropyCells[RandStream.RandRange(0, LowestEntropyCells.Num() - 1)];

        // Collapse the chosen cell
        FWFC_Cell& CellToCollapse = Grid[CellToCollapseIndex];
        int32 ChosenModuleIndex = CellToCollapse.PossibleModuleIndices[RandStream.RandRange(0, CellToCollapse.PossibleModuleIndices.Num() - 1)];
        CellToCollapse.PossibleModuleIndices.Empty();
        CellToCollapse.PossibleModuleIndices.Add(ChosenModuleIndex);
        CellToCollapse.bIsCollapsed = true;
        Assembly.PlacedModules[CellToCollapseIndex] = Modules[ChosenModuleIndex].ModuleId;

        PropagationStack.Add(CellToCollapseIndex);

        // 3. Propagation
        while (PropagationStack.Num() > 0)
        {
            int32 CurrentCellIndex = PropagationStack.Pop();
            // ... (Full propagation logic would go here)
            // This simplified version does not implement full propagation
        }
    }

    return Assembly;
}
'''
