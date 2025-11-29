# Blueprint Usage Guide: ProceduralAnimator

**Module:** PCG | Animation
**C++ Class:** `UProceduralAnimator`

## 1. Functionality

The `ProceduralAnimator` is a Blueprint Function Library for generating procedural 2D animations. It allows you to create dynamic animations for 2D skeletons, avoiding the need for pre-made animation assets.

## 2. Blueprint Usage Example

### Generating a Walk Animation

This function generates a procedural walk animation for a 2D skeleton.

**Node:** `Generate Walk Animation`

**Usage:**
1. Create a `F2DSkeleton` struct, defining your creature's bones and their hierarchy.
2. Call the `Generate Walk Animation` function, passing in the skeleton, speed, and amplitude.
3. The function will return an array of transforms, which you can use to drive the bone transforms in your animation blueprint.

## 3. Parameter Description

### GenerateWalkAnimation
| Parameter | Type | Description |
| :--- | :--- | :--- |
| **Skeleton** | `F2DSkeleton` | The 2D skeleton to animate. |
| **Speed** | `float` | The speed of the walk cycle. |
| **Amplitude** | `float` | The amplitude of the bone movements. |
| **Return Value** | `TArray<FTransform>` | An array of transforms representing the animation frames. |

## 4. Important Notes

- The current implementation is a simplified placeholder using sine waves. A full implementation would require more advanced techniques like IK and physics-based animation.
- The `F2DSkeleton` and `F2DBone` structs are the core of this system. Ensure that your bone hierarchy is correctly defined.
