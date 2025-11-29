# 魔力露珠物理系统 (Marble Physics System)

## 概述

物理系统是《回声炼金师》的核心模块，负责管理所有魔力露珠和魔药的物理模拟。支持炼金工作台和战斗场景的差异化物理规则。

## 核心类

### 1. UMarblePhysicsSystem
物理系统的核心类，负责管理所有魔力露珠的物理模拟。

### 2. FPhysicsSceneConfig
场景配置结构体，定义物理规则（重力、边界等）。

### 3. FMarbleState
魔力露珠状态结构体，存储位置、速度、生命周期等信息。

### 4. USceneConfigFactory
场景配置工厂，提供快速创建预设配置的函数。

## 蓝图使用指南

### 初始化场景

```
// 1. 创建物理系统实例
PhysicsSystem = NewObject<UMarblePhysicsSystem>();

// 2. 创建战斗场景配置
FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(
    FVector(-1000, -1000, 0),  // 边界最小点
    FVector(1000, 1000, 1000)  // 边界最大点
);

// 3. 初始化场景
PhysicsSystem->InitializeScene(Config);
```

### 发射魔力露珠

```
// 1. 创建发射参数
FMarbleLaunchParams Params;
Params.LaunchPosition = GetActorLocation();
Params.LaunchDirection = GetActorForwardVector();
Params.LaunchSpeed = 1000.0f;
Params.Potency = 5.0f;  // 战斗场景专用

// 2. 发射
FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);
```

### 更新物理（在Tick中）

```
void AYourActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 更新物理系统
    PhysicsSystem->Tick(DeltaTime);
    
    // 获取所有魔力露珠状态
    TArray<FMarbleState> Marbles = PhysicsSystem->GetAllMarbles();
    
    // 更新视觉表现
    for (const FMarbleState& Marble : Marbles)
    {
        UpdateMarbleVisual(Marble.ID, Marble.Position, Marble.Velocity);
    }
}
```

### 查询状态

```
// 获取魔力露珠数量
int32 Count = PhysicsSystem->GetMarbleCount();

// 检查是否所有魔力露珠都已停止
bool bAllStopped = PhysicsSystem->AreAllMarblesStopped();

// 获取特定魔力露珠的状态
FMarbleState State;
if (PhysicsSystem->GetMarbleState(MarbleID, State))
{
    // 使用状态数据
}
```

## 场景差异

### 炼金工作台场景
- ✅ 启用重力
- ✅ 边界行为：删除
- ✅ 不使用药效强度系统
- ✅ 高精度物理（不使用粒子优化）

### 战斗场景
- ✅ 无重力
- ✅ 边界行为：反弹
- ✅ 使用药效强度系统
- ✅ 支持粒子优化（大量魔药时）

## 性能优化

### 分级降级策略
- **第0代**（玩家发射）：使用Actor，高精度
- **第1代**（第一次分裂）：使用Actor，高精度
- **第2代及以上**：使用Niagara粒子系统，高性能

### 配置建议
- 炼金工作台：`MaxActorMarbles = 10`，`bEnableParticleOptimization = false`
- 战斗场景：`MaxActorMarbles = 10`，`bEnableParticleOptimization = true`

## 注意事项

1. **初始化顺序**：必须先调用 `InitializeScene` 才能使用其他功能
2. **Tick调用**：每帧必须调用 `Tick` 来更新物理状态
3. **职责分离**：物理系统只负责逻辑，视觉表现由蓝图负责
4. **碰撞检测**：当前版本不包含碰撞检测，将在VOI-25中实现

## 后续任务

- **VOI-25**: 实现碰撞检测系统
- **VOI-26**: 实现混合物理系统（Actor + Niagara）
- **VOI-27**: 实现GDD特殊物理效果
- **VOI-28**: 集成测试

## 版本信息

- **版本**: 1.0.0
- **任务**: VOI-24
- **作者**: Manus
- **日期**: 2025-11-29
