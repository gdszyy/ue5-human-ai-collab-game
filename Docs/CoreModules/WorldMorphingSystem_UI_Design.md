# WorldMorphingSystem UI设计文档

本文档详细说明WorldMorphingSystem的UI界面设计和实现。

## UI概览

WorldMorphingSystem的UI分为三个主要区域:

1. **统计信息面板** (左上角) - 显示实时模拟状态
2. **控制面板** (右侧) - 参数调整和操作按钮
3. **图例面板** (左下角) - 颜色说明

## 详细设计

### 1. 统计信息面板

#### 布局

```
┌─ Stats Panel ─────────────┐
│ Time Step: 1234           │
│ Cycle: 12                 │
│ Grid Size: 80x80          │
│ FPS: 60                   │
│                           │
│ Cell Statistics:          │
│ ├─ Terrain: 3200/6400     │
│ ├─ Alpha: 450             │
│ ├─ Beta: 120              │
│ ├─ Human: 5               │
│ └─ Thunderstorm: 23       │
└───────────────────────────┘
```

#### Widget结构

```
Canvas Panel
  └─ VerticalBox_Stats
      ├─ TextBlock_TimeStep
      │   ├─ Text: "Time Step: {TimeStep}"
      │   ├─ Font Size: 16
      │   └─ Color: White
      │
      ├─ TextBlock_CycleCount
      │   ├─ Text: "Cycle: {CycleCount}"
      │   ├─ Font Size: 16
      │   └─ Color: White
      │
      ├─ TextBlock_GridSize
      │   ├─ Text: "Grid: {Width}x{Height}"
      │   ├─ Font Size: 16
      │   └─ Color: White
      │
      ├─ TextBlock_FPS
      │   ├─ Text: "FPS: {FPS}"
      │   ├─ Font Size: 16
      │   └─ Color: (Green if >30, Yellow if >15, Red otherwise)
      │
      ├─ Spacer (Height: 20)
      │
      ├─ TextBlock_StatsTitle
      │   ├─ Text: "Cell Statistics:"
      │   ├─ Font Size: 14
      │   └─ Color: Yellow
      │
      ├─ TextBlock_Terrain
      │   ├─ Text: "Terrain: {Count}/{Total}"
      │   ├─ Font Size: 14
      │   └─ Color: White
      │
      ├─ TextBlock_Alpha
      │   ├─ Text: "Alpha: {Count}"
      │   ├─ Font Size: 14
      │   └─ Color: Lime
      │
      ├─ TextBlock_Beta
      │   ├─ Text: "Beta: {Count}"
      │   ├─ Font Size: 14
      │   └─ Color: Red
      │
      ├─ TextBlock_Human
      │   ├─ Text: "Human: {Count}"
      │   ├─ Font Size: 14
      │   └─ Color: Cyan
      │
      └─ TextBlock_Thunderstorm
          ├─ Text: "Thunderstorm: {Count}"
          ├─ Font Size: 14
          └─ Color: Yellow
```

### 2. 控制面板

#### 布局

```
┌─ Control Panel ───────────────────┐
│ ┌─ Simulation ─────────────────┐  │
│ │ Update Speed: [====|====] 1.0│  │
│ │ [Pause] [Reset] [Step]       │  │
│ └──────────────────────────────┘  │
│                                   │
│ ┌─ Mantle Parameters ──────────┐  │
│ │ Expansion Threshold:         │  │
│ │ [========|==] 120.0          │  │
│ │                              │  │
│ │ Edge Energy:                 │  │
│ │ [=====|=====] 10.0           │  │
│ └──────────────────────────────┘  │
│                                   │
│ ┌─ Climate Parameters ─────────┐  │
│ │ Diffusion Rate:              │  │
│ │ [====|======] 0.3            │  │
│ │                              │  │
│ │ Thunderstorm Threshold:      │  │
│ │ [=======|===] 20.0           │  │
│ └──────────────────────────────┘  │
│                                   │
│ ┌─ Crystal Parameters ─────────┐  │
│ │ Alpha Energy Demand:         │  │
│ │ [===|=======] 2.0            │  │
│ │                              │  │
│ │ Expansion Cost:              │  │
│ │ [======|====] 15.0           │  │
│ └──────────────────────────────┘  │
│                                   │
│ ┌─ View Options ───────────────┐  │
│ │ Color Mode:                  │  │
│ │ ○ Mantle Energy              │  │
│ │ ● Temperature                │  │
│ │ ○ Crystal Type               │  │
│ │                              │  │
│ │ [☑] Show Thunderstorm        │  │
│ │ [☑] Show Grid                │  │
│ └──────────────────────────────┘  │
└───────────────────────────────────┘
```

#### Widget结构

```
Canvas Panel
  └─ ScrollBox_Controls (右侧, Anchors: Right)
      │
      ├─ VerticalBox_Simulation
      │   ├─ TextBlock_Title ("Simulation")
      │   ├─ HorizontalBox_UpdateSpeed
      │   │   ├─ TextBlock ("Update Speed:")
      │   │   ├─ Slider_UpdateSpeed
      │   │   │   ├─ Min: 0.1
      │   │   │   ├─ Max: 5.0
      │   │   │   └─ Value: 1.0
      │   │   └─ TextBlock_UpdateSpeedValue
      │   │
      │   └─ HorizontalBox_Buttons
      │       ├─ Button_Pause
      │       │   └─ Text: "Pause"
      │       ├─ Button_Reset
      │       │   └─ Text: "Reset"
      │       └─ Button_Step
      │           └─ Text: "Step"
      │
      ├─ Spacer (Height: 20)
      │
      ├─ VerticalBox_MantleParams
      │   ├─ TextBlock_Title ("Mantle Parameters")
      │   ├─ HorizontalBox_ExpansionThreshold
      │   │   ├─ TextBlock ("Expansion Threshold:")
      │   │   ├─ Slider_ExpansionThreshold
      │   │   │   ├─ Min: 50.0
      │   │   │   ├─ Max: 200.0
      │   │   │   └─ Value: 120.0
      │   │   └─ TextBlock_ExpansionThresholdValue
      │   │
      │   └─ HorizontalBox_EdgeEnergy
      │       ├─ TextBlock ("Edge Energy:")
      │       ├─ Slider_EdgeEnergy
      │       │   ├─ Min: 1.0
      │       │   ├─ Max: 20.0
      │       │   └─ Value: 10.0
      │       └─ TextBlock_EdgeEnergyValue
      │
      ├─ Spacer (Height: 20)
      │
      ├─ VerticalBox_ClimateParams
      │   ├─ TextBlock_Title ("Climate Parameters")
      │   ├─ HorizontalBox_DiffusionRate
      │   │   ├─ TextBlock ("Diffusion Rate:")
      │   │   ├─ Slider_DiffusionRate
      │   │   │   ├─ Min: 0.1
      │   │   │   ├─ Max: 1.0
      │   │   │   └─ Value: 0.3
      │   │   └─ TextBlock_DiffusionRateValue
      │   │
      │   └─ HorizontalBox_ThunderstormThreshold
      │       ├─ TextBlock ("Thunderstorm Threshold:")
      │       ├─ Slider_ThunderstormThreshold
      │       │   ├─ Min: 5.0
      │       │   ├─ Max: 50.0
      │       │   └─ Value: 20.0
      │       └─ TextBlock_ThunderstormThresholdValue
      │
      ├─ Spacer (Height: 20)
      │
      ├─ VerticalBox_CrystalParams
      │   ├─ TextBlock_Title ("Crystal Parameters")
      │   ├─ HorizontalBox_AlphaEnergyDemand
      │   │   ├─ TextBlock ("Alpha Energy Demand:")
      │   │   ├─ Slider_AlphaEnergyDemand
      │   │   │   ├─ Min: 0.5
      │   │   │   ├─ Max: 5.0
      │   │   │   └─ Value: 2.0
      │   │   └─ TextBlock_AlphaEnergyDemandValue
      │   │
      │   └─ HorizontalBox_ExpansionCost
      │       ├─ TextBlock ("Expansion Cost:")
      │       ├─ Slider_ExpansionCost
      │       │   ├─ Min: 5.0
      │       │   ├─ Max: 30.0
      │       │   └─ Value: 15.0
      │       └─ TextBlock_ExpansionCostValue
      │
      ├─ Spacer (Height: 20)
      │
      └─ VerticalBox_ViewOptions
          ├─ TextBlock_Title ("View Options")
          ├─ RadioButton_MantleEnergy
          │   └─ Text: "Mantle Energy"
          ├─ RadioButton_Temperature
          │   └─ Text: "Temperature"
          ├─ RadioButton_CrystalType
          │   └─ Text: "Crystal Type"
          ├─ CheckBox_ShowThunderstorm
          │   └─ Text: "Show Thunderstorm"
          └─ CheckBox_ShowGrid
              └─ Text: "Show Grid"
```

### 3. 图例面板

#### 布局

```
┌─ Legend ──────────────────┐
│ Color Legend:             │
│ ■ Empty (Gray)            │
│ ■ Alpha Crystal (Green)   │
│ ■ Beta Crystal (Red)      │
│ ■ Human Settlement (Blue) │
│ ★ Thunderstorm (Yellow)   │
└───────────────────────────┘
```

#### Widget结构

```
Canvas Panel
  └─ VerticalBox_Legend (左下角, Anchors: Bottom-Left)
      ├─ TextBlock_Title
      │   ├─ Text: "Color Legend:"
      │   ├─ Font Size: 14
      │   └─ Color: Yellow
      │
      ├─ HorizontalBox_Empty
      │   ├─ Image_Empty
      │   │   ├─ Color: Gray
      │   │   └─ Size: 16x16
      │   └─ TextBlock ("Empty")
      │
      ├─ HorizontalBox_Alpha
      │   ├─ Image_Alpha
      │   │   ├─ Color: Lime
      │   │   └─ Size: 16x16
      │   └─ TextBlock ("Alpha Crystal")
      │
      ├─ HorizontalBox_Beta
      │   ├─ Image_Beta
      │   │   ├─ Color: Red
      │   │   └─ Size: 16x16
      │   └─ TextBlock ("Beta Crystal")
      │
      ├─ HorizontalBox_Human
      │   ├─ Image_Human
      │   │   ├─ Color: Cyan
      │   │   └─ Size: 16x16
      │   └─ TextBlock ("Human Settlement")
      │
      └─ HorizontalBox_Thunderstorm
          ├─ Image_Thunderstorm
          │   ├─ Color: Yellow
          │   └─ Size: 16x16
          └─ TextBlock ("Thunderstorm")
```

## 交互逻辑

### 1. 实时更新

```cpp
// Event Tick
void UWBPWorldMorphingUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if (Controller)
    {
        // 更新统计信息
        UpdateStats();
        
        // 更新FPS显示
        UpdateFPS(InDeltaTime);
    }
}
```

### 2. 参数同步

```cpp
// Slider值改变时
void UWBPWorldMorphingUI::OnExpansionThresholdChanged(float Value)
{
    if (Controller)
    {
        // 获取当前参数
        FSimulationParams Params = Controller->GetSimulationParams();
        
        // 修改参数
        Params.ExpansionThreshold = Value;
        
        // 应用参数
        Controller->SetSimulationParams(Params);
        
        // 更新显示
        TextBlock_ExpansionThresholdValue->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Value)));
    }
}
```

### 3. 按钮响应

```cpp
// 暂停/继续
void UWBPWorldMorphingUI::OnPauseClicked()
{
    if (Controller)
    {
        Controller->bAutoUpdate = !Controller->bAutoUpdate;
        
        // 更新按钮文本
        FText ButtonText = Controller->bAutoUpdate ? FText::FromString("Pause") : FText::FromString("Resume");
        Button_Pause->SetText(ButtonText);
    }
}

// 重置
void UWBPWorldMorphingUI::OnResetClicked()
{
    if (Controller)
    {
        Controller->ResetWorld();
    }
}

// 单步执行
void UWBPWorldMorphingUI::OnStepClicked()
{
    if (Controller)
    {
        Controller->TickSimulation(0.016f);
    }
}
```

### 4. 颜色模式切换

```cpp
// 颜色模式改变
void UWBPWorldMorphingUI::OnColorModeChanged(EColorMode NewMode)
{
    if (Visualizer)
    {
        Visualizer->SetColorMode(NewMode);
    }
}
```

## 样式设置

### 颜色方案

```cpp
// 主题颜色
const FLinearColor PrimaryColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.8f);  // 深灰色背景
const FLinearColor AccentColor = FLinearColor(0.2f, 0.6f, 1.0f, 1.0f);   // 蓝色强调
const FLinearColor TextColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);     // 白色文字
const FLinearColor WarningColor = FLinearColor(1.0f, 0.8f, 0.0f, 1.0f);  // 黄色警告
const FLinearColor ErrorColor = FLinearColor(1.0f, 0.2f, 0.2f, 1.0f);    // 红色错误

// 晶石颜色
const FLinearColor EmptyColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);    // 灰色
const FLinearColor AlphaColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);    // 绿色
const FLinearColor BetaColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);     // 红色
const FLinearColor HumanColor = FLinearColor(0.0f, 1.0f, 1.0f, 1.0f);    // 青色
```

### 字体设置

```cpp
// 标题字体
Font Family: Roboto
Font Size: 18
Font Style: Bold

// 正文字体
Font Family: Roboto
Font Size: 14
Font Style: Regular

// 数值字体
Font Family: Roboto Mono
Font Size: 14
Font Style: Regular
```

## 响应式布局

### 屏幕尺寸适配

```cpp
// 根据屏幕尺寸调整UI缩放
void UWBPWorldMorphingUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 获取屏幕尺寸
    FVector2D ViewportSize;
    GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
    
    // 计算缩放比例
    float Scale = FMath::Min(ViewportSize.X / 1920.0f, ViewportSize.Y / 1080.0f);
    
    // 应用缩放
    SetRenderScale(FVector2D(Scale, Scale));
}
```

## 性能优化

### 1. 减少更新频率

```cpp
// 统计信息每0.1秒更新一次
float StatsUpdateInterval = 0.1f;
float TimeSinceLastStatsUpdate = 0.0f;

void UWBPWorldMorphingUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    TimeSinceLastStatsUpdate += InDeltaTime;
    
    if (TimeSinceLastStatsUpdate >= StatsUpdateInterval)
    {
        UpdateStats();
        TimeSinceLastStatsUpdate = 0.0f;
    }
}
```

### 2. 缓存文本格式

```cpp
// 缓存格式化字符串
FText TimeStepFormat = FText::FromString("Time Step: {0}");
FText CycleCountFormat = FText::FromString("Cycle: {0}");

// 使用缓存的格式
TextBlock_TimeStep->SetText(FText::Format(TimeStepFormat, TimeStep));
```

## 辅助功能

### 1. 键盘快捷键

```cpp
// 在PlayerController中处理输入
void APlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    InputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &APlayerController::TogglePause);
    InputComponent->BindKey(EKeys::R, IE_Pressed, this, &APlayerController::ResetWorld);
    InputComponent->BindKey(EKeys::S, IE_Pressed, this, &APlayerController::StepSimulation);
    InputComponent->BindKey(EKeys::One, IE_Pressed, this, &APlayerController::SetColorMode_MantleEnergy);
    InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &APlayerController::SetColorMode_Temperature);
    InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &APlayerController::SetColorMode_CrystalType);
}
```

### 2. 工具提示

为每个控件添加Tooltip:

```cpp
// 设置Tooltip
Slider_ExpansionThreshold->SetToolTipText(
    FText::FromString("Controls when terrain expands. Higher values make expansion harder.")
);
```

## 总结

本UI设计提供了:

1. ✅ **清晰的信息展示** - 实时统计和状态
2. ✅ **直观的参数控制** - Slider和按钮
3. ✅ **灵活的视图选项** - 多种颜色模式
4. ✅ **响应式布局** - 适配不同屏幕
5. ✅ **性能优化** - 减少不必要的更新
6. ✅ **辅助功能** - 键盘快捷键和工具提示

按照本设计文档，您可以在UE4的UMG编辑器中创建完整的UI界面。
