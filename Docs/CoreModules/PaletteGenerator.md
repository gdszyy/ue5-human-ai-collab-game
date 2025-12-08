# 调色板生成器 (Palette Generator)

**版本**: 2.0  
**最后更新**: 2025-12-08  
**模块**: PCG (Procedural Content Generation)  
**状态**: 已实现

---

## 概述

调色板生成器是怪兽生成系统的核心组件之一,负责根据怪兽的生态属性和战斗属性自动生成和谐的配色方案。该系统采用算法驱动的方式,确保怪兽的颜色能够准确反映其内在属性和所属环境。

### 核心价值

调色板生成器通过自动化配色过程,实现了以下目标。首先,它实现了属性驱动外观的设计原则,使怪兽的颜色直接反映其生态属性(栖息地、体型)和战斗属性(稀有度、特殊能力)。其次,它确保了视觉一致性,相同栖息地的怪兽具有相似的色调,增强世界的真实感。第三,通过颜色的饱和度和亮度可以快速识别怪兽的稀有度。第四,通过高光色提示怪兽的特殊能力(如火属性、冰属性)。最后,它减少了手动配色的工作量,支持大规模程序化生成。

---

## 设计原则

### 属性驱动外观

调色板生成器遵循"属性驱动外观"(ADA)原则,即怪兽的外观必须直接、清晰地反映其核心属性。栖息地决定基础色调,森林怪兽偏绿色,沙漠怪兽偏黄色。体型影响颜色明度,大型怪兽颜色更深,小型怪兽颜色更亮。稀有度影响饱和度,稀有怪兽颜色更鲜艳。战斗属性添加强调色,火属性怪兽带有红橙色高光。

### HSV色彩空间

系统使用HSV色彩空间进行颜色计算,而非RGB空间。HSV空间更符合人类对颜色的感知,便于进行色相调整(改变颜色的基本色调)、饱和度调整(控制颜色的鲜艳程度)和明度调整(控制颜色的明暗程度)。

### 确定性生成

所有调色板生成都基于随机种子(Seed),确保相同的输入参数和种子总是产生相同的调色板。这对于可复现性、调试测试和网络同步至关重要。

---

## 核心功能

### 生态栖息地色谱系统

每种栖息地都有预定义的色谱范围,定义了该栖息地怪兽的基础颜色特征。

**栖息地色谱定义**

| 栖息地 | 色相范围 | 饱和度范围 | 明度范围 | 特征 |
|--------|----------|------------|----------|------|
| 森林 | 90°-150° | 0.4-0.7 | 0.3-0.8 | 绿色和棕色调,中等饱和度 |
| 沙漠 | 30°-60° | 0.3-0.6 | 0.6-0.95 | 黄橙色调,高明度 |
| 冰原 | 180°-240° | 0.2-0.5 | 0.7-1.0 | 蓝白色调,低饱和度,高明度 |
| 洞穴 | 260°-300° | 0.2-0.5 | 0.2-0.5 | 紫灰色调,低明度 |
| 火山 | 0°-30° | 0.7-1.0 | 0.5-0.9 | 红橙色调,高饱和度 |
| 沼泽 | 60°-120° | 0.3-0.6 | 0.2-0.5 | 暗绿色调,低明度 |

除了栖息地,其他生态属性也会影响最终的颜色。耐受值影响饱和度,耐受值高的怪兽颜色更鲜艳。体型影响明度,大型怪兽颜色更深。

### 稀有度颜色映射

稀有度通过调整颜色的饱和度和明度来体现,使玩家能够快速识别怪兽的稀有程度。

| 稀有度 | 饱和度调整 | 明度调整 | 视觉效果 |
|--------|------------|----------|----------|
| 普通 | 无调整 | 无调整 | 正常颜色 |
| 稀有 | +20% | 无调整 | 更鲜艳 |
| 史诗 | +40% | +20% | 非常鲜艳且明亮 |

### 战斗属性颜色修饰

战斗词条会为怪兽添加特殊的强调色,通常体现在高光色上。系统预定义了常见战斗词条的强调色,包括火属性(红橙色)、冰属性(青蓝色)、毒属性(毒绿色)、石属性(灰褐色)、雷属性(亮黄色)、暗属性(暗紫色)、光属性(亮白黄)、血属性(深红色)和风属性(浅蓝色)。

### LUT纹理生成

调色板可以转换为LUT(Look-Up Table)纹理,用于材质系统中的颜色重映射。LUT纹理是一个1D纹理,包含从阴影色到高光色的平滑渐变。

---

## 调色板结构

每个生成的调色板包含四种核心颜色:主色(怪兽的主导颜色,占据60-70%)、辅色(细节和变化,占据20-30%)、高光色(强调和特殊能力,占据5-10%)和阴影色(增加深度和立体感)。

---

## 使用方法

### 蓝图使用示例

**从怪兽属性生成完整调色板**

这是最常用的方法,适用于完整的怪兽生成流程。

```
Monster Attributes = ...
Palette = Generate Palette From Monster Attributes(Monster Attributes, Seed: 12345)
Set Material Color Parameter(Material, "PrimaryColor", Palette.Primary Color)
Set Material Color Parameter(Material, "HighlightColor", Palette.Highlight Color)
```

**仅从生态属性生成基础调色板**

适用于需要先生成基础颜色,后续再根据战斗属性调整的场景。

```
Ecology Attributes = ...
Base Palette = Generate Palette From Ecology(Ecology Attributes, Seed: 67890)
Combat Attributes = ...
Final Palette = Modify Palette With Combat Attributes(Base Palette, Combat Attributes, Seed: 11111)
```

**获取栖息地色谱**

```
Spectrum = Get Habitat Color Spectrum(Habitat: Forest)
Print String("Hue Range: " + Spectrum.Hue Range)
```

**生成LUT纹理**

```
Palette = Generate Palette From Monster Attributes(Monster Attributes, Seed: 12345)
LUT Texture = Generate LUT Texture From Palette(Palette, Texture Size: 256)
Set Material Texture Parameter(Material, "ColorLUT", LUT Texture)
```

### C++使用示例

```cpp
FMonsterAttributes MonsterAttrs;
MonsterAttrs.EcologyAttributes.Habitat = EHabitatType::Forest;
MonsterAttrs.EcologyAttributes.SizeClass = ESizeClass::Large;

FCombatTrait FireTrait;
FireTrait.TraitID = FName(TEXT("FireAffinity"));
FireTrait.Rarity = ERarity::Epic;
MonsterAttrs.CombatAttributes.Traits.Add(FireTrait);

FPalette Palette = UPaletteGenerator::GeneratePaletteFromMonsterAttributes(MonsterAttrs, 12345);

UMaterialInstanceDynamic* DynMaterial = Monster->GetMaterial();
DynMaterial->SetVectorParameterValue(FName(TEXT("PrimaryColor")), Palette.PrimaryColor);
```

---

## 算法流程

调色板生成分为两个主要阶段。第一阶段基于生态属性生成基础调色板,包括获取栖息地色谱、生成主色、应用耐受值和体型调整、生成辅色高光色和阴影色。第二阶段基于战斗属性修改调色板,包括确定最高稀有度、应用稀有度调整、混合词条强调色。

---

## 性能考虑

调色板生成是一个轻量级操作,时间复杂度为O(n),其中n是战斗词条数量(通常1-3个),空间复杂度为O(1),典型耗时小于0.1ms。在大规模生成场景中可以考虑缓存调色板、批量生成和预计算LUT。LUT纹理生成相对较慢,典型耗时1-5ms,建议在开发阶段预生成并保存为资产。

---

## 测试

调色板生成器包含全面的单元测试,覆盖栖息地色谱、生态调色板、体型影响、稀有度调整、词条强调色、完整生成、战斗修改和兼容性等所有核心功能。在UE4编辑器中运行自动化测试:

```
Session Frontend > Automation > EchoAlchemist.PCG.PaletteGenerator
```

---

## 扩展和定制

要添加新的栖息地类型,需要在`MonsterAttributes.h`中添加枚举值,在`GetHabitatColorSpectrum`函数中添加色谱定义,并添加测试用例。要为新的战斗词条添加强调色,只需在`GetTraitAccentColor`函数中添加关键词匹配。要自定义稀有度调整,可以修改`AdjustColorByRarity`函数中的调整参数。

---

## 最佳实践

始终使用确定性种子来生成调色板,使用怪兽ID作为种子而不是随机种子。对于常用的调色板,建议在开发阶段预生成LUT纹理并保存为资产。不要为怪兽添加过多的战斗词条(建议1-3个),否则强调色会混合得过于复杂。

---

## 相关模块

- **MonsterAttributes** - 定义怪兽的生态属性和战斗属性
- **AttributeGenerator** - 生成怪兽属性
- **AppearanceAssembler** - 使用调色板组装怪兽外观
- **MonsterVisualizer** - 将调色板应用到怪兽的视觉表现

---

## 更新日志

### v2.0 (2025-12-08)

实现算法驱动的调色板生成系统,添加生态栖息地色谱定义(6种栖息地)、稀有度颜色调整(普通/稀有/史诗)、战斗词条强调色系统(9种常见词条类型)、LUT纹理生成功能和完整的单元测试套件(8个测试类别)。

### v1.0 (2025-11-29)

初始实现,包含基础的色谱生成和单色调色板生成。

---

**维护者**: Manus AI  
**相关任务**: VOI-20, VOI-11
