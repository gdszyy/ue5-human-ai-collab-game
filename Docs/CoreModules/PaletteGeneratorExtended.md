# Palette Generator Extended - 算法驱动的调色板生成器

**模块：** PCG | Palette Extended  
**C++类：** `UPaletteGeneratorExtended`  
**版本：** 1.0  
**最后更新：** 2025-12-08

---

## 1. 功能说明

`PaletteGeneratorExtended` 是一个扩展的调色板生成器，专门为怪兽生成系统设计。它能够根据怪兽的**生态属性**（栖息地、体型、耐受值等）和**战斗属性**（词条）自动生成和谐的配色方案。

### 核心特性

- ✅ **栖息地色谱定义**：为6种栖息地预定义了科学的色谱范围
- ✅ **智能颜色选择**：基于生态属性和战斗属性的算法驱动颜色生成
- ✅ **稀有度调整**：稀有和史诗怪兽自动获得更鲜艳的颜色
- ✅ **词条影响**：战斗词条（如火属性、冰属性）会影响颜色倾向
- ✅ **调色板扩展**：可将3色调色板扩展为多色渐变
- ✅ **确定性生成**：相同种子生成相同调色板，支持复现

---

## 2. 数据结构

### 2.1 FHabitatColorSpectrum

栖息地色谱结构，定义了某个栖息地的颜色范围。

```cpp
USTRUCT(BlueprintType)
struct FHabitatColorSpectrum
{
    GENERATED_BODY()

    /** 栖息地类型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    EHabitatType Habitat;

    /** 主色色谱（身体颜色） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    TArray<FLinearColor> PrimarySpectrum;

    /** 次色色谱（细节颜色） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    TArray<FLinearColor> SecondarySpectrum;

    /** 强调色色谱（高光颜色） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    TArray<FLinearColor> AccentSpectrum;
};
```

### 2.2 FMonsterColorPalette

怪兽颜色调色板，包含主色、次色和强调色。

```cpp
USTRUCT(BlueprintType)
struct FMonsterColorPalette
{
    GENERATED_BODY()

    /** 主色（主要身体颜色） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor PrimaryColor;

    /** 次色（细节颜色） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor SecondaryColor;

    /** 强调色（高光颜色） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor AccentColor;
};
```

---

## 3. 蓝图使用示例

### 3.1 生成怪兽调色板

这是最常用的功能，根据怪兽的完整属性生成调色板。

**节点：** `Generate Monster Palette`

![Generate Monster Palette](https://i.imgur.com/example.png) *（图片占位符）*

**使用方法：**

```
// 步骤1：准备怪兽属性
Ecology Attributes → (Habitat: Forest, Size: Medium, Tolerance: 50.0)
Combat Attributes → (Traits: [FireAffinity (Rare)])

// 步骤2：生成调色板
Generate Monster Palette(Ecology Attributes, Combat Attributes, Seed: 12345) → Monster Color Palette

// 步骤3：使用调色板
Monster Color Palette → Primary Color → Set Material Parameter (PrimaryColor)
Monster Color Palette → Secondary Color → Set Material Parameter (SecondaryColor)
Monster Color Palette → Accent Color → Set Material Parameter (AccentColor)
```

**参数说明：**

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Ecology Attributes** | `FEcologyAttributes` | 怪兽的生态属性（栖息地、体型等） |
| **Combat Attributes** | `FCombatAttributes` | 怪兽的战斗属性（词条） |
| **Seed** | `int32` | 随机种子（相同种子生成相同调色板） |
| **Return Value** | `FMonsterColorPalette` | 生成的怪兽调色板 |

---

### 3.2 获取栖息地色谱

查询某个栖息地的预定义色谱。

**节点：** `Get Habitat Color Spectrum`

![Get Habitat Color Spectrum](https://i.imgur.com/example2.png) *（图片占位符）*

**使用方法：**

```
// 获取森林栖息地的色谱
Get Habitat Color Spectrum(Habitat: Forest) → Habitat Color Spectrum

// 查看主色色谱
Habitat Color Spectrum → Primary Spectrum → For Each Loop → Print Color
```

**参数说明：**

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Habitat** | `EHabitatType` | 栖息地类型（Forest, Desert, Tundra, Cave, Volcano, Swamp） |
| **Return Value** | `FHabitatColorSpectrum` | 该栖息地的色谱 |

---

### 3.3 根据稀有度调整调色板

手动调整调色板的稀有度，使颜色更鲜艳。

**节点：** `Adjust Palette By Rarity`

![Adjust Palette By Rarity](https://i.imgur.com/example3.png) *（图片占位符）*

**使用方法：**

```
// 创建基础调色板
Base Palette → (Primary: Gray, Secondary: Dark Gray, Accent: Light Gray)

// 调整为史诗稀有度
Adjust Palette By Rarity(Base Palette, Rarity: Epic) → Adjusted Palette

// 结果：颜色变得更鲜艳和明亮
```

**参数说明：**

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Base Palette** | `FMonsterColorPalette` | 基础调色板 |
| **Rarity** | `ERarity` | 稀有度（Common, Rare, Epic） |
| **Return Value** | `FMonsterColorPalette` | 调整后的调色板 |

**稀有度效果：**

- **Common**：无调整
- **Rare**：饱和度 +20%，亮度 +10%
- **Epic**：饱和度 +40%，亮度 +20%

---

### 3.4 根据战斗词条调整调色板

根据战斗词条（如火属性、冰属性）调整颜色倾向。

**节点：** `Adjust Palette By Traits`

![Adjust Palette By Traits](https://i.imgur.com/example4.png) *（图片占位符）*

**使用方法：**

```
// 创建基础调色板
Base Palette → (Primary: Green, Secondary: Dark Green, Accent: Yellow)

// 创建火属性词条
Fire Trait → (Trait ID: "FireAffinity", Rarity: Rare)

// 调整调色板
Adjust Palette By Traits(Base Palette, Traits: [Fire Trait]) → Fire Palette

// 结果：强调色偏向红橙色
```

**参数说明：**

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Base Palette** | `FMonsterColorPalette` | 基础调色板 |
| **Traits** | `TArray<FCombatTrait>` | 战斗词条数组 |
| **Return Value** | `FMonsterColorPalette` | 调整后的调色板 |

**词条颜色影响：**

| 词条类型 | 颜色倾向 | 示例 |
| :--- | :--- | :--- |
| **Fire / Flame** | 红橙色 | `FireAffinity` → 红橙 |
| **Ice / Frost** | 冰蓝色 | `IceBreath` → 冰蓝 |
| **Poison / Toxic** | 毒绿色 | `PoisonSpit` → 毒绿 |
| **Stone / Rock** | 灰色 | `StoneShell` → 灰色 |
| **Lightning / Thunder** | 电蓝色 | `LightningStrike` → 电蓝 |
| **Shadow / Dark** | 暗紫色 | `ShadowCloak` → 暗紫 |
| **Light / Holy** | 明黄色 | `HolyAura` → 明黄 |

---

### 3.5 扩展怪兽调色板

将3色调色板扩展为多色渐变，用于创建颜色变化。

**节点：** `Expand Monster Palette`

![Expand Monster Palette](https://i.imgur.com/example5.png) *（图片占位符）*

**使用方法：**

```
// 创建怪兽调色板
Monster Palette → (Primary: Red, Secondary: Green, Accent: Blue)

// 扩展为8色调色板
Expand Monster Palette(Monster Palette, Num Colors: 8) → Expanded Palette

// 使用扩展调色板
Expanded Palette → Colors → For Each Loop → Apply To Material
```

**参数说明：**

| 参数 | 类型 | 描述 |
| :--- | :--- | :--- |
| **Monster Palette** | `FMonsterColorPalette` | 怪兽调色板（3色） |
| **Num Colors** | `int32` | 希望生成的颜色数量 |
| **Return Value** | `FPalette` | 扩展后的调色板 |

---

## 4. 栖息地色谱定义

### 4.1 Forest（森林）

**主色：** 绿色、棕色、大地色调  
**次色：** 棕色、浅棕色、橄榄绿  
**强调色：** 黄绿色、金色、青柠色

**适用怪兽：** 森林生物、植物类怪兽、自然元素怪兽

---

### 4.2 Desert（沙漠）

**主色：** 沙色、橙色、暖色调  
**次色：** 陶土色、粘土色、尘土色  
**强调色：** 橙色、金色、琥珀色

**适用怪兽：** 沙漠生物、岩石类怪兽、热带怪兽

---

### 4.3 Tundra（冰原）

**主色：** 冰蓝色、白色、冷色调  
**次色：** 雪白色、霜色、冰灰色  
**强调色：** 明白色、青色、淡青色

**适用怪兽：** 冰原生物、冰霜类怪兽、寒冷元素怪兽

---

### 4.4 Cave（洞穴）

**主色：** 深灰色、石色、暗紫灰色  
**次色：** 浅灰色、板岩色、暗紫色  
**强调色：** 水晶紫色、水晶蓝色、矿物橙色

**适用怪兽：** 洞穴生物、矿物类怪兽、地下怪兽

---

### 4.5 Volcano（火山）

**主色：** 熔岩红色、鲜红色、暗红色  
**次色：** 木炭色、灰烬色、焦黑色  
**强调色：** 明橙色、黄橙色、火橙色

**适用怪兽：** 火山生物、火焰类怪兽、熔岩怪兽

---

### 4.6 Swamp（沼泽）

**主色：** 浑浊绿色、苔藓绿色、沼泽绿色  
**次色：** 泥棕色、湿土色、暗苔藓色  
**强调色：** 毒绿色、病态黄色、明苔藓色

**适用怪兽：** 沼泽生物、毒素类怪兽、腐败怪兽

---

## 5. 完整工作流示例

### 5.1 在怪兽生成时应用调色板

```
// 事件：生成怪兽
Event Begin Play

// 步骤1：生成怪兽属性
Generate Monster Attributes(Seed: 12345, Climate: 0.7, Terrain: 0.5, Num Traits: 2) → Monster Attributes

// 步骤2：生成调色板
Generate Monster Palette(
    Monster Attributes → Ecology Attributes,
    Monster Attributes → Combat Attributes,
    Seed: 12345
) → Monster Color Palette

// 步骤3：创建动态材质实例
Get Mesh Component → Create Dynamic Material Instance(0) → Material Instance

// 步骤4：应用调色板到材质
Material Instance → Set Vector Parameter Value(Name: "PrimaryColor", Value: Monster Color Palette → Primary Color)
Material Instance → Set Vector Parameter Value(Name: "SecondaryColor", Value: Monster Color Palette → Secondary Color)
Material Instance → Set Vector Parameter Value(Name: "AccentColor", Value: Monster Color Palette → Accent Color)

// 步骤5：设置怪兽属性
Self → Set Monster Attributes(Monster Attributes)
```

---

### 5.2 根据世界状态动态调整颜色

```
// 事件：世界状态变化
Event On World State Changed(New Climate, New Terrain)

// 步骤1：重新生成怪兽属性
Generate Monster Attributes(Saved Seed, New Climate, New Terrain, 2) → New Monster Attributes

// 步骤2：重新生成调色板
Generate Monster Palette(
    New Monster Attributes → Ecology Attributes,
    New Monster Attributes → Combat Attributes,
    Saved Seed
) → New Monster Color Palette

// 步骤3：平滑过渡到新颜色
Timeline → Lerp(Old Palette, New Palette, Alpha) → Current Palette

// 步骤4：应用到材质
Material Instance → Set Vector Parameter Value(Name: "PrimaryColor", Value: Current Palette → Primary Color)
Material Instance → Set Vector Parameter Value(Name: "SecondaryColor", Value: Current Palette → Secondary Color)
Material Instance → Set Vector Parameter Value(Name: "AccentColor", Value: Current Palette → Accent Color)
```

---

## 6. 材质设置指南

### 6.1 创建调色板替换主材质

在UE4材质编辑器中创建 `M_PaletteSwap_Master` 材质：

**节点结构：**

```
[TextureSample: BaseTexture]
    ↓
[If: Pixel.R == 1.0 && Pixel.G == 0.0 && Pixel.B == 0.0]  // 红色 = 主色
    → [VectorParameter: PrimaryColor]
[Else If: Pixel.R == 0.0 && Pixel.G == 1.0 && Pixel.B == 0.0]  // 绿色 = 次色
    → [VectorParameter: SecondaryColor]
[Else If: Pixel.R == 0.0 && Pixel.G == 0.0 && Pixel.B == 1.0]  // 蓝色 = 强调色
    → [VectorParameter: AccentColor]
[Else]
    → [Original Color]
    ↓
[FinalColor]
```

**参数说明：**

- `PrimaryColor`：主色（身体）
- `SecondaryColor`：次色（细节）
- `AccentColor`：强调色（高光）

---

### 6.2 准备精灵纹理

在精灵纹理中使用特定颜色标记需要替换的区域：

- **红色 (R=1, G=0, B=0)**：主色区域（身体）
- **绿色 (R=0, G=1, B=0)**：次色区域（细节）
- **蓝色 (R=0, G=0, B=1)**：强调色区域（高光）

---

## 7. 性能考虑

### 7.1 调色板生成性能

- **生成时间：** < 1ms（单个怪兽）
- **内存占用：** 极小（仅3个颜色值）
- **确定性：** 相同种子保证相同结果

### 7.2 优化建议

1. **缓存调色板**：对于相同属性的怪兽，缓存调色板避免重复生成
2. **批量生成**：在关卡加载时批量生成所有怪兽的调色板
3. **异步生成**：对于大量怪兽，可以在后台线程生成调色板

---

## 8. 测试验证

### 8.1 单元测试

运行自动化测试以验证功能：

```bash
# 在UE4编辑器中
Window → Developer Tools → Session Frontend → Automation
# 搜索 "PaletteGeneratorExtended"
# 运行所有测试
```

**测试覆盖：**

- ✅ 获取栖息地色谱
- ✅ 生成怪兽调色板
- ✅ 根据稀有度调整调色板
- ✅ 根据战斗词条调整调色板
- ✅ 扩展怪兽调色板

---

### 8.2 视觉测试

在编辑器中创建测试关卡：

1. 创建测试关卡 `TestMap_PaletteGenerator`
2. 放置多个怪兽Actor
3. 为每个怪兽设置不同的栖息地和词条
4. 观察颜色是否符合预期

**预期结果：**

- 森林怪兽：绿色、棕色调
- 沙漠怪兽：沙色、橙色调
- 火属性怪兽：红橙色强调色
- 稀有怪兽：更鲜艳的颜色

---

## 9. 常见问题

### Q1: 如何让怪兽颜色更鲜艳？

**A:** 增加稀有度或使用 `AdjustPaletteByRarity` 函数手动调整。

```
Adjust Palette By Rarity(Base Palette, Rarity: Epic) → Brighter Palette
```

---

### Q2: 如何自定义栖息地色谱？

**A:** 当前色谱是硬编码的。如需自定义，可以：

1. 修改 `GetHabitatColorSpectrum` 函数中的颜色定义
2. 或者创建数据表存储自定义色谱

---

### Q3: 调色板生成是否确定性？

**A:** 是的。相同的种子、生态属性和战斗属性会生成相同的调色板。

---

### Q4: 如何在材质中应用调色板？

**A:** 使用动态材质实例：

```
Create Dynamic Material Instance → Set Vector Parameter Value(Name: "PrimaryColor", Value: Palette → Primary Color)
```

---

## 10. 未来扩展

### 10.1 LUT纹理生成

当前 `CreateLUTTextureFromPalette` 函数返回 `nullptr`。未来可以实现：

- 生成真实的LUT纹理
- 支持导出为资源文件
- 支持在编辑器中预览

---

### 10.2 自定义色谱

未来可以添加：

- 数据表驱动的色谱定义
- 编辑器工具用于可视化编辑色谱
- 色谱导入导出功能

---

## 11. 参考资料

- [PaletteGenerator.md](PaletteGenerator.md) - 基础调色板生成器
- [MonsterAttributes.h](../Source/EchoAlchemist/Public/PCG/MonsterAttributes.h) - 怪兽属性定义
- [MHP_VOI-11_Monster_Generation_System.md](../MHP_VOI-11_Monster_Generation_System.md) - 怪兽生成系统交接文档

---

**最后更新：** 2025-12-08  
**维护者：** Manus AI  
**版本：** 1.0
