# WorldMorphingSystem 快速开始指南

本指南帮助您在5分钟内开始使用WorldMorphingSystem。

## 前置条件

- ✅ UE4 4.27 已安装
- ✅ 项目已克隆并打开
- ✅ C++代码已编译成功

## 步骤1: 自动创建蓝图资产 (1分钟)

在UE4编辑器的Output Log中执行：

```
py "Scripts/create_world_morphing_blueprints.py"
```

这将自动创建所有必需的蓝图和材质。

## 步骤2: 实现Controller逻辑 (2分钟)

### 2.1 打开BP_WorldMorphingController

在Content Browser中导航到：
```
/Game/Blueprints/Systems/WorldMorphing/BP_WorldMorphingController
```

### 2.2 添加变量

在Variables面板中添加：

| 变量名 | 类型 | 默认值 |
| :--- | :--- | :--- |
| GridWidth | Integer | 50 |
| GridHeight | Integer | 50 |

### 2.3 实现Event BeginPlay

在Event Graph中添加节点：

```
Event BeginPlay
  → Make Default Params
    → Initialize World
      - Context: Get Game Instance
      - Width: GridWidth
      - Height: GridHeight
      - Params: (连接Make Default Params的返回值)
```

### 2.4 实现Event Tick

```
Event Tick
  → Tick Simulation
    - Context: Get Game Instance
    - Delta Time: Delta Seconds
```

### 2.5 编译并保存

点击 "Compile" 和 "Save"。

## 步骤3: 创建测试关卡 (2分钟)

### 3.1 打开WorldMorphingTest关卡

在Content Browser中导航到：
```
/Game/Maps/TestMaps/WorldMorphingTest
```

### 3.2 放置Actor

1. 从Content Browser拖入 `BP_WorldMorphingController`
   - 位置: (0, 0, 0)

2. 添加相机：
   - 放置 Camera Actor
   - 位置: (2500, 2500, 4000)
   - 旋转: Pitch = -45
   - 设置 Auto Possess Player 0

3. 添加光源：
   - 放置 Directional Light
   - 旋转: Pitch = -45, Yaw = 45

### 3.3 保存关卡

点击 "Save Current"。

## 步骤4: 运行测试 (30秒)

### 4.1 点击Play按钮

在编辑器工具栏点击 "Play"。

### 4.2 运行Python测试

在Output Log中输入：

```
py "Scripts/test_world_morphing.py"
```

### 4.3 验证结果

检查Output Log，应该看到：

```
✅ 世界已初始化: 50x50
✅ 更新完成，当前时间步: 10
✅ 统计完成
🎉 所有测试通过！
```

## 恭喜！🎉

您已成功运行WorldMorphingSystem！

## 下一步

### 添加可视化

按照 [蓝图实现指南](WorldMorphingSystem_Blueprint_Guide.md#bp_worldmorphingvisualizer-实现) 实现3D可视化。

### 添加UI

按照 [UI设计文档](WorldMorphingSystem_UI_Design.md) 创建交互界面。

### 深入学习

- [使用指南](WorldMorphingSystem.md) - 详细的API文档
- [实现总结](WorldMorphingSystem_Implementation.md) - 算法细节
- [测试指南](WorldMorphingSystem_Testing_Guide.md) - 完整测试流程

## 常见问题

### Q: 测试失败怎么办？

**A**: 检查以下几点：
1. 是否在PIE模式下运行？
2. C++代码是否编译成功？
3. Python插件是否启用？

### Q: 看不到蓝图节点？

**A**: 
1. 重新编译C++代码
2. 重启UE4编辑器
3. 检查UFUNCTION宏

### Q: 性能不佳？

**A**: 
1. 减小网格尺寸（GridWidth/GridHeight）
2. 降低更新频率
3. 参考 [性能优化建议](WorldMorphingSystem.md#性能优化)

## 获取帮助

如有问题，请参考：
- [完整文档](WorldMorphingSystem.md)
- [测试指南](WorldMorphingSystem_Testing_Guide.md)
- [问题排查](WorldMorphingSystem_Testing_Guide.md#问题排查)
