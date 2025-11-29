# 蓝图使用指南：ShapeGenerator

**模块：** PCG | Shape
**C++类：** `UShapeGenerator`

## 1. 功能说明

`ShapeGenerator` 是一个蓝图函数库，用于程序化地生成2D形状。它提供了多种算法，可以根据不同的生态属性生成有机或平滑的形状，为怪兽的基础轮廓提供多样性。

## 2. 蓝图使用示例

### 2.1 使用细胞自动机生成形状

此函数可以生成有机、不对称的形状，适合用于血肉或异形类怪兽。

**节点：** `Generate Shape with Cellular Automata`

**使用方法：**
1. 调用`Generate Shape with Cellular Automata`函数，传入网格尺寸、随机种子和迭代次数。
2. 函数将返回一个`ShapeData`结构体，其中包含了生成的形状网格。

### 2.2 使用Simplex噪声生成形状

此函数可以生成平滑、有流动感的形状，适合用于元素或能量类怪兽。

**节点：** `Generate Shape with Simplex Noise`

**使用方法：**
1. 调用`Generate Shape with Simplex Noise`函数，传入网格尺寸、随机种子、噪声缩放和阈值。
2. 函数将返回一个`ShapeData`结构体。

## 3. 参数说明

### GenerateShapeWithCellularAutomata
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Width/Height** | `int32` | 形状网格的尺寸。 |
| **Seed** | `int32` | 随机种子。 |
| **Iterations** | `int32` | 模拟迭代次数。 |
| **Birth/SurvivalThreshold** | `int32` | 细胞诞生/存活的邻居数量阈值。 |

### GenerateShapeWithSimplexNoise
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Width/Height** | `int32` | 形状网格的尺寸。 |
| **Seed** | `int32` | 噪声的随机种子。 |
| **Scale** | `float` | 噪声图案的缩放。 |
| **Threshold** | `float` | 决定细胞是否为固体的阈值。 |

## 4. 注意事项

- `ShapeData`中的`Grid`是一个一维数组，需要使用`y * Width + x`来访问二维坐标。
- 细胞自动机和Simplex噪声的参数可以极大地影响最终形状，建议多加尝试不同的参数组合。

### 2.3 使用Voronoi图生成形状

此函数可以生成几何化、棱角分明的形状，适合用于晶石或矿物类怪兽。

**节点：** `Generate Shape with Voronoi`

**使用方法：**
1. 调用`Generate Shape with Voronoi`函数，传入网格尺寸、随机种子和点数。
2. 函数将返回一个`ShapeData`结构体。

### 2.4 使用晶体生长算法生成形状

此函数可以模拟晶体从种子点向外生长的过程，生成对称、放射状的形状。

**节点：** `Generate Shape with Crystal Growth`

**使用方法：**
1. 调用`Generate Shape with Crystal Growth`函数，传入网格尺寸、随机种子、迭代次数和生长概率。
2. 函数将返回一个`ShapeData`结构体。

## 3. 参数说明 (续)

### GenerateShapeWithVoronoi
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Width/Height** | `int32` | 形状网格的尺寸。 |
| **Seed** | `int32` | 随机种子。 |
| **NumPoints** | `int32` | Voronoi图的点数。 |

### GenerateShapeWithCrystalGrowth
| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Width/Height** | `int32` | 形状网格的尺寸。 |
| **Seed** | `int32` | 随机种子。 |
| **Iterations** | `int32` | 生长迭代次数。 |
| **GrowthChance** | `float` | 新晶体形成的概率。 |
