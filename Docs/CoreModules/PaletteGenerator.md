# 蓝图使用指南：PaletteGenerator

**模块：** PCG | Palette
**C++类：** `UPaletteGenerator`

## 1. 功能说明

`PaletteGenerator` 是一个蓝图函数库，用于程序化地生成颜色调色板。它提供了多种算法，可以根据给定的颜色、色谱或参数生成和谐的配色方案。这对于实现怪兽、环境和特效的动态颜色变化非常有用。

## 2. 蓝图使用示例

### 2.1 从色谱生成调色板

此函数可以从一个预定义的颜色数组（色谱）中，通过插值生成一个新的调色板。

**节点：** `Generate Palette From Spectrum`

![Generate Palette From Spectrum](https://i.imgur.com/example.png)  *（图片占位符）*

**使用方法：**
1. 创建一个`LinearColor`数组，作为你的基础色谱。
2. 调用`Generate Palette From Spectrum`函数，传入色谱和希望生成的颜色数量。
3. 函数将返回一个`Palette`结构体，其中包含了生成的颜色数组。

### 2.2 生成单色调色板

此函数可以从一个基础颜色生成一个单色调色板，通过调整饱和度和亮度来创建变化。

**节点：** `Generate Monochromatic Palette`

![Generate Monochromatic Palette](https://i.imgur.com/example2.png) *（图片占位符）*

**使用方法：**
1. 提供一个基础颜色。
2. 调用`Generate Monochromatic Palette`函数，可以调整饱和度和亮度的变化范围。
3. 函数将返回一个`Palette`结构体。

## 3. 参数说明

### GeneratePaletteFromSpectrum
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Spectrum** | `TArray<FLinearColor>` | 输入的基础颜色数组（色谱）。 |
| **NumColors** | `int32` | 希望在调色板中生成的颜色数量。 |
| **Return Value** | `FPalette` | 输出的调色板结构体。 |

### GenerateMonochromaticPalette
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **BaseColor** | `FLinearColor` | 输入的基础颜色。 |
| **NumColors** | `int32` | 希望在调色板中生成的颜色数量。 |
| **SaturationRange** | `FVector2D` | 饱和度变化范围（X=Min, Y=Max）。 |
| **ValueRange** | `FVector2D` | 亮度变化范围（X=Min, Y=Max）。 |
| **Return Value** | `FPalette` | 输出的调色板结构体。 |

## 4. 注意事项

- `GeneratePaletteFromSpectrum` 使用HSV色彩空间进行插值，这通常比RGB插值能产生更自然的颜色过渡。
- 如果输入的`Spectrum`为空或`NumColors`为0，将返回一个空的调色板。
- `GenerateMonochromaticPalette` 的`SaturationRange`和`ValueRange`的取值范围为0到1。
