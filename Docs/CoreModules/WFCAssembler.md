# Blueprint Usage Guide: WFCAssembler

**Module:** PCG | WFC
**C++ Class:** `UWFCAssembler`

## 1. Functionality

The `WFCAssembler` is a Blueprint Function Library that uses the Wave Function Collapse (WFC) algorithm to intelligently assemble creatures from a library of modular parts. It ensures that parts are connected in a logical way, based on predefined connector rules.

## 2. Blueprint Usage Example

### Assembling a Creature with WFC

This function takes a library of modules and assembles them into a creature.

**Node:** `Assemble with WFC`

**Usage:**
1. Create an array of `WFCModule` structs, defining your creature's parts and their connectors.
2. Call the `Assemble with WFC` function, passing in the module library, grid dimensions, and a random seed.
3. The function will return a `WFCAssembly` struct, which contains the IDs of the placed modules in a grid.

## 3. Parameter Description

### AssembleWithWFC
| Parameter | Type | Description |
| :--- | :--- | :--- |
| **Modules** | `TArray<FWFCModule>` | The library of modules to use for assembly. |
| **Width/Height** | `int32` | The dimensions of the assembly grid. |
| **Seed** | `int32` | A random seed for the assembly process. |
| **Return Value** | `FWFCAssembly` | The final assembled creature. |

## 4. Important Notes

- The current implementation is a placeholder and places modules randomly. A full WFC implementation is required for intelligent assembly.
- The `FWFCConnector` struct is crucial for defining how modules connect. Ensure that your connector types and directions are consistent.


## 5. Algorithm Details (Updated)

The updated `AssembleWithWFC` function now implements the core logic of the Wave Function Collapse algorithm:

1.  **Initialization**: Each cell in the grid is initialized with all possible modules.
2.  **Observation**: The algorithm iteratively finds the cell with the lowest entropy (fewest possible modules) and collapses it to a single, randomly chosen module.
3.  **Propagation**: After a cell is collapsed, constraints are propagated to its neighbors, removing any modules that are no longer compatible. (Note: Full propagation logic is still in development).

This process continues until all cells are collapsed, resulting in a fully assembled, logically consistent creature.
