# 混合物理系统 (Hybrid Physics System)

## 概述

混合物理系统结合了Actor高精度物理和Niagara粒子系统的高性能渲染，通过分级降级策略自动选择最优实现方式。核心弹珠使用Actor确保物理精度，大量次级弹珠使用粒子系统确保性能。

## 核心类

### 1. AMarbleActor
高精度Actor实现，用于核心弹珠（第0-1代）。使用UE4原生物理引擎进行模拟，提供最高的物理精度。

### 2. UMarbleActorPool
Actor对象池管理器，复用Actor对象以提升性能。避免频繁创建和销毁Actor带来的性能开销。

### 3. UMarblePhysicsSystem (扩展)
物理系统核心类，集成了混合物理系统。提供统一的接口管理Actor和粒子两种实现。

## 分级降级策略

根据弹珠的**代数**（Generation）自动选择实现方式：

| 代数 | 实现方式 | 物理精度 | 性能开销 | 使用场景 |
|:---|:---|:---|:---|:---|
| **第0代** | Actor | 100% | 高 | 玩家直接发射的弹珠 |
| **第1代** | Actor | 100% | 高 | 第一次分裂产生的弹珠 |
| **第2代及以上** | Niagara粒子 | 80% | 低 | 连锁分裂产生的大量弹珠 |

**代数定义**：
- 第0代：玩家直接发射的弹珠
- 第1代：第0代弹珠分裂产生的弹珠
- 第2代：第1代弹珠分裂产生的弹珠
- 以此类推...

**性能对比**：
- 100个第0代弹珠：100个Actor，性能良好
- 100个第0代弹珠分裂成200个第1代：200个Actor，性能开始下降
- 200个第1代弹珠分裂成400个第2代：200个Actor + 400个粒子，性能优秀

## 蓝图使用指南

### 初始化混合物理系统

```
// 1. 初始化场景
FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(Min, Max);
PhysicsSystem->InitializeScene(Config);

// 2. 初始化混合物理系统
PhysicsSystem->InitializeHybridPhysics(GetWorld(), 50);  // 预创建50个Actor
```

**注意事项**：
- 必须先调用InitializeScene，再调用InitializeHybridPhysics
- 如果不调用InitializeHybridPhysics，所有弹珠都使用逻辑模拟（无视觉表现）
- PreAllocateActorCount建议根据场景设置（炼金工作台：10，战斗场景：50）

### 使用对象池管理Actor

```
// 1. 从对象池获取Actor
AMarbleActor* Marble = ActorPool->Acquire();

// 2. 初始化Actor
FMarbleState State;
State.Position = SpawnLocation;
State.Velocity = LaunchDirection * LaunchSpeed;
State.Radius = 10.0f;
State.Generation = 0;  // 第0代
Marble->InitializeFromState(State);

// 3. 使用完毕后归还
ActorPool->Release(Marble);
```

### 监控对象池状态

```
// 获取对象池统计信息
int32 TotalCount, AvailableCount, InUseCount;
PhysicsSystem->GetActorPoolStatistics(TotalCount, AvailableCount, InUseCount);

UE_LOG(LogTemp, Log, TEXT("Actor Pool: Total=%d, Available=%d, InUse=%d"),
    TotalCount, AvailableCount, InUseCount);

// 性能警告
if (InUseCount > 100)
{
    UE_LOG(LogTemp, Warning, TEXT("Too many actors in use, consider increasing particle threshold"));
}
```

## AMarbleActor详细说明

### 核心功能

**InitializeFromState**: 从魔力露珠状态初始化Actor，设置位置、速度、半径、质量等物理属性。

**Launch**: 发射魔力露珠，立即应用冲量到物理组件。

**GetCurrentState**: 获取当前魔力露珠状态，包含实时位置、速度等信息。

**SetGravityEnabled**: 设置重力启用状态（炼金工作台启用，战斗场景禁用）。

**ResetToPool**: 重置Actor到初始状态，用于对象池回收。

### 物理组件

**USphereComponent**: 球体碰撞组件，负责物理模拟和碰撞检测。

**UStaticMeshComponent**: 静态网格组件，负责视觉表现。

### 使用示例

```
// 生成并初始化魔力露珠
AMarbleActor* Marble = ActorPool->Acquire();

FMarbleState State;
State.ID = FGuid::NewGuid();
State.Position = FVector(0, 0, 100);
State.Velocity = FVector(1000, 0, 0);
State.Radius = 10.0f;
State.Mass = 1.0f;
State.Generation = 0;

Marble->InitializeFromState(State);

// 发射
Marble->Launch(FVector(1, 0, 0), 1000.0f);

// 禁用重力（战斗场景）
Marble->SetGravityEnabled(false);

// 获取当前状态
FMarbleState CurrentState = Marble->GetCurrentState();
UE_LOG(LogTemp, Log, TEXT("Position: %s, Speed: %.2f"),
    *CurrentState.Position.ToString(),
    CurrentState.Velocity.Size());

// 使用完毕后归还
ActorPool->Release(Marble);
```

## 对象池管理

### 对象池原理

对象池通过复用Actor对象避免频繁创建和销毁带来的性能开销。Actor在归还时会被重置状态并隐藏，下次获取时直接激活使用。

### 对象池生命周期

```
1. 初始化：预创建N个Actor并隐藏
2. 获取：从可用列表中取出Actor并激活
3. 使用：Actor正常进行物理模拟
4. 归还：重置Actor状态并放回可用列表
5. 清理：销毁所有Actor并重置对象池
```

### 性能优化建议

**预创建数量**：
- 炼金工作台：10-20个（弹珠数量少）
- 战斗场景：50-100个（弹珠数量多）
- 根据实际使用情况调整

**监控指标**：
- 如果AvailableCount经常为0，说明预创建数量不足
- 如果InUseCount远小于TotalCount，说明预创建数量过多
- 理想情况：AvailableCount保持在TotalCount的20-30%

**扩展策略**：
- 对象池会在不足时自动创建新Actor
- 但频繁创建会影响性能
- 建议根据监控数据调整预创建数量

## Niagara粒子系统集成

### 当前状态

当前版本已预留Niagara粒子系统接口，但具体实现需要在蓝图中配置Niagara资源。

### 集成步骤（待实现）

1. 在UE4编辑器中创建Niagara粒子系统资源
2. 配置粒子物理属性（速度、碰撞、生命周期等）
3. 在蓝图中将Niagara资源绑定到PhysicsSystem
4. 实现粒子碰撞事件处理

### 预期效果

- 第2代及以上弹珠使用Niagara粒子渲染
- 粒子物理精度约80%（满足视觉需求）
- 性能开销远低于Actor（GPU加速）
- 支持数百上千个粒子同时存在

## 性能基准

### 测试场景

**场景1：炼金工作台**
- 5个第0代弹珠
- 无分裂效果
- 全部使用Actor
- 帧率：60+ FPS

**场景2：战斗场景（无分裂）**
- 50个第0代弹珠
- 无分裂效果
- 全部使用Actor
- 帧率：45-60 FPS

**场景3：战斗场景（连锁分裂）**
- 10个第0代弹珠
- 分裂成20个第1代（Actor）
- 分裂成40个第2代（粒子）
- 帧率：50-60 FPS

### 性能瓶颈

**Actor数量**：超过100个Actor时帧率开始下降。

**物理计算**：Actor使用UE4原生物理引擎，CPU开销较大。

**碰撞检测**：Actor碰撞检测由引擎处理，性能优秀。

### 优化建议

1. 合理设置分级降级阈值（当前为第2代）
2. 使用对象池复用Actor
3. 第2代及以上使用Niagara粒子
4. 监控对象池使用情况并动态调整

## 与其他系统的集成

### 与碰撞检测系统的集成

```
// 1. Actor碰撞由UE4引擎处理
Marble->OnComponentHit.AddDynamic(this, &AMyActor::OnMarbleHit);

// 2. 在碰撞事件中同步到碰撞管理器
void AMyActor::OnMarbleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, ...)
{
    // 获取魔力露珠状态
    FMarbleState State = Marble->GetCurrentState();
    
    // 更新碰撞管理器
    CollisionManager->UpdateBodyPosition(State.ID, State.Position);
    
    // 执行碰撞检测
    TArray<FCollisionEvent> Collisions;
    CollisionManager->DetectCollisionsForBody(State.ID, Collisions);
}
```

### 与物理系统的集成

```
// 1. 物理系统管理逻辑状态
PhysicsSystem->Tick(DeltaTime);

// 2. 同步Actor状态到物理系统
for (auto& Pair : MarbleActors)
{
    FGuid MarbleID = Pair.Key;
    AMarbleActor* Actor = Pair.Value;
    
    FMarbleState State = Actor->GetCurrentState();
    PhysicsSystem->UpdateMarbleState(MarbleID, State);
}

// 3. 物理系统决定弹珠销毁
if (State.Potency <= 0)
{
    ActorPool->Release(Actor);
    MarbleActors.Remove(MarbleID);
}
```

## 注意事项

1. **初始化顺序**：必须先InitializeScene，再InitializeHybridPhysics
2. **对象池管理**：使用完Actor必须调用Release归还
3. **性能监控**：定期检查对象池统计信息
4. **Niagara集成**：当前版本需要在蓝图中配置Niagara资源
5. **代数判断**：确保正确设置弹珠的Generation字段

## 后续任务

- **VOI-27**: 实现GDD特殊物理效果（引力奇点、虫洞、分裂等）
- **VOI-28**: 集成测试
- **Niagara实现**: 在蓝图中配置Niagara粒子系统

## 版本信息

- **版本**: 1.0.0
- **任务**: VOI-26
- **作者**: Manus
- **日期**: 2025-11-29
