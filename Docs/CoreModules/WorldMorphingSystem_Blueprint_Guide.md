# WorldMorphingSystem 蓝图实现指南

本文档提供详细的蓝图实现步骤，帮助您在UE4中创建完整的世界变迁系统可视化和交互。

## 目录

1. [自动创建蓝图资产](#自动创建蓝图资产)
2. [BP_WorldMorphingController 实现](#bp_worldmorphingcontroller-实现)
3. [BP_WorldMorphingVisualizer 实现](#bp_worldmorphingvisualizer-实现)
4. [WBP_WorldMorphingUI 实现](#wbp_worldmorphingui-实现)
5. [测试关卡设置](#测试关卡设置)

---

## 自动创建蓝图资产

### 步骤1: 运行自动创建脚本

在UE4编辑器的Output Log中执行:

```
py "Scripts/create_world_morphing_blueprints.py"
```

这将自动创建以下资产:
- `BP_WorldMorphingController` - 控制器Actor
- `BP_WorldMorphingVisualizer` - 可视化Actor
- `WBP_WorldMorphingUI` - UI Widget
- `M_WorldMorphingCell` - 材质
- `WorldMorphingTest` - 测试关卡

---

## BP_WorldMorphingController 实现

### 功能说明

控制器负责初始化世界、执行模拟更新、管理参数。

### 变量定义

在蓝图的Variables面板中添加:

| 变量名 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- |
| `GridWidth` | Integer | 80 | 网格宽度 |
| `GridHeight` | Integer | 80 | 网格高度 |
| `SimulationParams` | SimulationParams | (默认) | 模拟参数 |
| `bAutoUpdate` | Boolean | True | 是否自动更新 |
| `UpdateSpeed` | Float | 1.0 | 更新速度倍率 |
| `Visualizer` | BP_WorldMorphingVisualizer | None | 可视化Actor引用 |

### Event Graph 实现

#### 1. Event BeginPlay

```
Event BeginPlay
  |
  ├─> Make Default Params
  |     └─> Set SimulationParams
  |
  ├─> Initialize World
  |     ├─ Context: Get Game Instance
  |     ├─ Width: GridWidth
  |     ├─ Height: GridHeight
  |     └─ Params: SimulationParams
  |
  └─> Print String ("World Morphing System Initialized")
```

**节点说明**:
1. 使用 `Make Default Params` 创建默认参数
2. 将参数存储到 `SimulationParams` 变量
3. 调用 `Initialize World` 初始化网格
4. 打印日志确认初始化成功

#### 2. Event Tick

```
Event Tick
  |
  ├─> Branch (bAutoUpdate?)
  |     |
  |     └─ True ─> Tick Simulation
  |                 ├─ Context: Get Game Instance
  |                 └─ Delta Time: Delta Seconds * UpdateSpeed
  |
  └─> (可选) 每N帧更新可视化
        └─> Call Visualizer->UpdateVisualization
```

**节点说明**:
1. 检查 `bAutoUpdate` 是否启用
2. 如果启用，调用 `Tick Simulation` 更新模拟
3. Delta Time 乘以 `UpdateSpeed` 实现变速
4. (可选) 定期调用Visualizer更新显示

#### 3. 自定义函数: ResetWorld

```
Function: ResetWorld
  |
  ├─> Make Default Params
  |     └─> Set SimulationParams
  |
  └─> Initialize World
        ├─ Context: Get Game Instance
        ├─ Width: GridWidth
        ├─ Height: GridHeight
        └─ Params: SimulationParams
```

**用途**: 重置世界到初始状态

#### 4. 自定义函数: GetWorldStats

```
Function: GetWorldStats
  |
  ├─> Get Grid Size
  |     ├─ Context: Get Game Instance
  |     └─> (Width, Height)
  |
  ├─> Get Time Step
  |     ├─ Context: Get Game Instance
  |     └─> TimeStep
  |
  ├─> Get Cycle Count
  |     ├─ Context: Get Game Instance
  |     └─> CycleCount
  |
  └─> Return (Width, Height, TimeStep, CycleCount)
```

**用途**: 获取世界统计信息，供UI显示

---

## BP_WorldMorphingVisualizer 实现

### 功能说明

可视化Actor负责将网格数据渲染为3D场景。

### 组件结构

在Components面板中添加:

```
DefaultSceneRoot (Scene Component)
  └─ CellMesh (Instanced Static Mesh Component)
```

**CellMesh 设置**:
- Static Mesh: `Cube` (或自定义网格)
- Material: `M_WorldMorphingCell`
- Mobility: Movable

### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- |
| `CellSize` | Float | 100.0 | 单元格大小 |
| `CellSpacing` | Float | 10.0 | 单元格间距 |
| `Controller` | BP_WorldMorphingController | None | 控制器引用 |
| `ColorMode` | Enum | Mantle Energy | 颜色模式 |

### Event Graph 实现

#### 1. Event BeginPlay

```
Event BeginPlay
  |
  └─> Find Actor of Class (BP_WorldMorphingController)
        └─> Set Controller
```

**节点说明**: 自动查找场景中的Controller

#### 2. 自定义函数: UpdateVisualization

```
Function: UpdateVisualization
  |
  ├─> Get Grid Size (from Controller)
  |     └─> (Width, Height)
  |
  ├─> Clear Instances (CellMesh)
  |
  └─> For Each (Y in 0 to Height-1)
        └─> For Each (X in 0 to Width-1)
              |
              ├─> Get Cell At (X, Y)
              |     └─> CellState
              |
              ├─> Branch (CellState.bExists?)
              |     |
              |     └─ True ─> Calculate Position
              |                 ├─ X: X * (CellSize + CellSpacing)
              |                 ├─ Y: Y * (CellSize + CellSpacing)
              |                 └─ Z: 0
              |
              ├─> Calculate Color (based on ColorMode)
              |     ├─ Mantle Energy: Lerp(Black, Red, Energy/100)
              |     ├─ Temperature: Lerp(Blue, Red, (Temp+50)/100)
              |     └─ Crystal Type: 
              |           ├─ Empty: Gray
              |           ├─ Alpha: Green
              |           ├─ Beta: Red
              |           └─ Human: Blue
              |
              └─> Add Instance (CellMesh)
                    ├─ Transform: Position, Rotation(0), Scale(1)
                    └─ Custom Data: Color (as Vertex Color)
```

**节点说明**:
1. 获取网格尺寸
2. 清除所有现有实例
3. 遍历所有单元格
4. 对于存在地形的单元格，添加实例
5. 根据ColorMode设置颜色

#### 3. 自定义函数: SetColorMode

```
Function: SetColorMode (Input: NewMode)
  |
  ├─> Set ColorMode = NewMode
  |
  └─> UpdateVisualization
```

**用途**: 切换颜色显示模式

---

## WBP_WorldMorphingUI 实现

### 功能说明

UI Widget显示模拟状态和提供参数调整。

### Designer 布局

#### 根容器: Canvas Panel

```
Canvas Panel
  ├─ VerticalBox_Stats (左上角)
  │   ├─ TextBlock_TimeStep
  │   ├─ TextBlock_CycleCount
  │   ├─ TextBlock_GridSize
  │   └─ TextBlock_FPS
  │
  ├─ VerticalBox_Controls (右侧)
  │   ├─ Slider_UpdateSpeed
  │   ├─ Slider_ExpansionThreshold
  │   ├─ Slider_ThunderstormThreshold
  │   ├─ Button_Reset
  │   └─ Button_TogglePause
  │
  └─ VerticalBox_Legend (左下角)
      ├─ TextBlock_Legend_Title
      ├─ HorizontalBox_Empty
      ├─ HorizontalBox_Alpha
      ├─ HorizontalBox_Beta
      └─ HorizontalBox_Human
```

### 变量定义

| 变量名 | 类型 | 说明 |
| :--- | :--- | :--- |
| `Controller` | BP_WorldMorphingController | 控制器引用 |
| `Visualizer` | BP_WorldMorphingVisualizer | 可视化器引用 |

### Event Graph 实现

#### 1. Event Construct

```
Event Construct
  |
  ├─> Find Actor of Class (BP_WorldMorphingController)
  |     └─> Set Controller
  |
  ├─> Find Actor of Class (BP_WorldMorphingVisualizer)
  |     └─> Set Visualizer
  |
  └─> Bind Events
        ├─ Button_Reset -> OnClicked
        ├─ Button_TogglePause -> OnClicked
        ├─ Slider_UpdateSpeed -> OnValueChanged
        ├─ Slider_ExpansionThreshold -> OnValueChanged
        └─ Slider_ThunderstormThreshold -> OnValueChanged
```

#### 2. Event Tick

```
Event Tick
  |
  ├─> Get World Stats (from Controller)
  |     └─> (Width, Height, TimeStep, CycleCount)
  |
  ├─> Set Text (TextBlock_TimeStep)
  |     └─ Format: "Time Step: {TimeStep}"
  |
  ├─> Set Text (TextBlock_CycleCount)
  |     └─ Format: "Cycle: {CycleCount}"
  |
  ├─> Set Text (TextBlock_GridSize)
  |     └─ Format: "Grid: {Width}x{Height}"
  |
  └─> Set Text (TextBlock_FPS)
        └─ Format: "FPS: {GetFrameRate}"
```

#### 3. Button_Reset OnClicked

```
OnClicked (Button_Reset)
  |
  └─> Call Controller->ResetWorld
```

#### 4. Button_TogglePause OnClicked

```
OnClicked (Button_TogglePause)
  |
  ├─> Get Controller->bAutoUpdate
  |
  ├─> NOT
  |
  └─> Set Controller->bAutoUpdate
```

#### 5. Slider_UpdateSpeed OnValueChanged

```
OnValueChanged (Slider_UpdateSpeed)
  |
  └─> Set Controller->UpdateSpeed = Value
```

#### 6. Slider_ExpansionThreshold OnValueChanged

```
OnValueChanged (Slider_ExpansionThreshold)
  |
  ├─> Get Simulation Params (from Controller)
  |
  ├─> Set ExpansionThreshold = Value
  |
  └─> Set Simulation Params (to Controller)
```

#### 7. Slider_ThunderstormThreshold OnValueChanged

```
OnValueChanged (Slider_ThunderstormThreshold)
  |
  ├─> Get Simulation Params (from Controller)
  |
  ├─> Set ThunderstormThreshold = Value
  |
  └─> Set Simulation Params (to Controller)
```

---

## 测试关卡设置

### 步骤1: 打开测试关卡

在Content Browser中导航到 `/Game/Maps/TestMaps/WorldMorphingTest`，双击打开。

### 步骤2: 放置Actor

1. **放置 BP_WorldMorphingController**
   - 从Content Browser拖入场景
   - 位置: (0, 0, 0)
   - 设置 GridWidth = 80, GridHeight = 80

2. **放置 BP_WorldMorphingVisualizer**
   - 从Content Browser拖入场景
   - 位置: (0, 0, 0)
   - 设置 CellSize = 100, CellSpacing = 10

3. **放置相机**
   - 添加 Camera Actor
   - 位置: (4000, 4000, 8000)
   - 旋转: Pitch = -45
   - 设置为 Auto Possess Player 0

4. **放置光源**
   - 添加 Directional Light
   - 旋转: Pitch = -45, Yaw = 45

### 步骤3: 设置GameMode

1. 打开 World Settings
2. 设置 GameMode Override = EchoAlchemistGameMode
3. 设置 HUD Class = WBP_WorldMorphingUI

### 步骤4: 测试

1. 点击 Play 按钮
2. 观察世界初始化
3. 观察模拟更新
4. 使用UI调整参数

---

## 高级功能

### 1. 相机控制

在PlayerController蓝图中添加:

```
Event Tick
  |
  ├─> Get Input Axis (Mouse Wheel)
  |     └─> Add Actor World Offset (Camera, Z = Value * 100)
  |
  ├─> Get Input Axis (Mouse X)
  |     └─> Add Actor World Rotation (Camera, Yaw = Value)
  |
  └─> Get Input Axis (Mouse Y)
        └─> Add Actor World Rotation (Camera, Pitch = Value)
```

### 2. 性能优化

在BP_WorldMorphingVisualizer中:

```
Variable: UpdateInterval (Float) = 0.1
Variable: TimeSinceLastUpdate (Float) = 0.0

Event Tick
  |
  ├─> TimeSinceLastUpdate += DeltaSeconds
  |
  └─> Branch (TimeSinceLastUpdate >= UpdateInterval?)
        |
        └─ True ─> UpdateVisualization
                   └─> TimeSinceLastUpdate = 0
```

### 3. 截图功能

添加按钮:

```
OnClicked (Button_Screenshot)
  |
  └─> Execute Console Command
        └─ Command: "HighResShot 2"
```

---

## 常见问题

### Q1: 蓝图节点找不到?

**A**: 确保项目已编译C++代码，WorldMorphingBlueprintLibrary的函数会自动暴露到蓝图。

### Q2: 可视化不显示?

**A**: 检查:
1. Controller是否正确初始化
2. Visualizer是否正确引用Controller
3. CellMesh是否设置了材质

### Q3: 性能问题?

**A**: 
1. 减小网格尺寸
2. 增加UpdateInterval
3. 使用LOD机制

### Q4: 参数调整无效?

**A**: 确保调用了 `Set Simulation Params` 将修改后的参数写回系统。

---

## 总结

按照本指南，您应该能够:

1. ✅ 自动创建所有必需的蓝图资产
2. ✅ 实现控制器逻辑
3. ✅ 实现可视化渲染
4. ✅ 创建交互式UI
5. ✅ 设置测试关卡

如有问题，请参考:
- [使用指南](WorldMorphingSystem.md)
- [实现总结](WorldMorphingSystem_Implementation.md)
- [Python测试脚本](../Scripts/test_world_morphing.py)
