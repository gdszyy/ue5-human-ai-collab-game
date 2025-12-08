# Linear任务与资源映射表（索引表）

**版本**: 1.0  
**最后更新**: 2025-11-29  
**维护者**: Manus AI

---

## 概述

本文档记录了所有Linear任务与协议仓库（`ue4-human-ai-collab`）、Game仓库（`ue4-human-ai-collab-game`）资源之间的映射关系，便于快速定位和追溯。

### 索引表的作用

1. **快速定位资源**：通过任务ID快速找到所有相关资源（TDD、MHP、代码、文档、测试）
2. **追溯历史**：记录任务的完整历史和演进过程
3. **避免重复**：避免重复创建相同的资源
4. **便于审查**：便于人类开发者审查任务的完整性
5. **知识沉淀**：将项目知识沉淀到Git仓库中

### 更新规则

- **任务创建时**：在Linear任务创建时，立即在索引表中添加一行记录
- **资源创建时**：在创建TDD、MHP、代码、文档、测试时，立即更新索引表
- **任务状态变更时**：在Linear任务状态变更时，更新索引表的状态列
- **对话结束前**：在对话结束前，确保索引表已更新

---

## 映射表

### 已完成的任务

| 任务ID | 任务标题 | 状态 | 优先级 | 协议仓库资源 | Game仓库资源 | 备注 |
|--------|----------|------|--------|--------------|--------------|------|
| VOI-5 | Git仓库配置 | Done | Medium | MHP: `docs/mhp/VOI-5_git_setup.md` | 无（基础设施任务） | Git仓库初始化 |
| VOI-6 | 世界变迁系统 | Done | High | TDD: `docs/tech/world_morphing_system_tdd.md`<br>MHP: `docs/mhp/VOI-6_world_morphing.md`<br>模拟器: `docs/tech/simulator/` | 代码: `Source/EchoAlchemist/Public/WorldMorphing/WorldMorphingSubsystem.h`<br>代码: `Source/EchoAlchemist/Private/WorldMorphing/WorldMorphingSubsystem.cpp`<br>代码: `Source/EchoAlchemist/Public/WorldMorphing/WorldMorphingBlueprintLibrary.h`<br>代码: `Source/EchoAlchemist/Private/WorldMorphing/WorldMorphingBlueprintLibrary.cpp`<br>代码: `Source/EchoAlchemist/Public/WorldMorphing/PerlinNoise.h`<br>代码: `Source/EchoAlchemist/Private/WorldMorphing/PerlinNoise.cpp`<br>文档: `Docs/CoreModules/WorldMorphingSystem.md`<br>文档: `Docs/CoreModules/WorldMorphingSystem_*.md`（6个文档）<br>测试: 无 | ✅ 完整实现，包含TypeScript模拟器<br>✅ 完美遵循四层架构<br>✅ 最佳实践典范 |
| VOI-24 | VOI-18.1: 实现物理系统核心框架 (UMarblePhysicsSystem) | Done | Urgent | 无 | 代码: `Source/EchoAlchemist/Public/Physics/MarblePhysicsSystem.h`<br>代码: `Source/EchoAlchemist/Private/Physics/MarblePhysicsSystem.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/PhysicsSceneConfig.h`<br>代码: `Source/EchoAlchemist/Private/Physics/PhysicsSceneConfig.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/MarbleState.h`<br>文档: `Docs/CoreModules/PhysicsSystem.md`<br>测试: `Source/EchoAlchemist/Tests/Physics/MarblePhysicsSystemTest.cpp` | ✅ 物理系统核心框架已完成<br>✅ 场景配置系统已实现<br>✅ 魔力露珠状态管理已实现<br>✅ 基础Tick物理更新循环已实现 |
| VOI-25 | VOI-18.2: 实现碰撞检测系统 (UCollisionManager) | Done | Urgent | 无 | 代码: `Source/EchoAlchemist/Public/Physics/CollisionManager.h`<br>代码: `Source/EchoAlchemist/Private/Physics/CollisionManager.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/CollisionShape.h`<br>代码: `Source/EchoAlchemist/Public/Physics/SpatialGrid.h`<br>代码: `Source/EchoAlchemist/Private/Physics/SpatialGrid.cpp`<br>文档: `Docs/CoreModules/CollisionSystem.md`<br>测试: `Source/EchoAlchemist/Tests/Physics/CollisionManagerTest.cpp` | ✅ 碰撞管理器已完成<br>✅ 空间网格优化已实现<br>✅ 圆-圆碰撞检测已实现<br>✅ 圆-矩形碰撞检测已实现（AABB）<br>✅ 碰撞事件管理已实现 |

### 进行中的任务

| 任务ID | 任务标题 | 状态 | 优先级 | 协议仓库资源 | Game仓库资源 | 备注 |
|--------|----------|------|--------|--------------|--------------|------|
| VOI-11 | 怪兽生成系统 | In Progress | High | TDD: `docs/tech/monster_generation_system_tdd.md` | 代码: `Source/EchoAlchemist/Public/PCG/PaletteGenerator.h`<br>代码: `Source/EchoAlchemist/Private/PCG/PaletteGenerator.cpp`<br>代码: `Source/EchoAlchemist/Public/PCG/ShapeGenerator.h`<br>代码: `Source/EchoAlchemist/Private/PCG/ShapeGenerator.cpp`<br>代码: `Source/EchoAlchemist/Public/PCG/ProceduralAnimator.h`<br>代码: `Source/EchoAlchemist/Private/PCG/ProceduralAnimator.cpp`<br>代码: `Source/EchoAlchemist/Public/PCG/WFCAssembler.h`<br>代码: `Source/EchoAlchemist/Private/PCG/WFCAssembler.cpp`<br>代码: `Source/EchoAlchemist/Public/PCG/AttributeGenerator.h`<br>代码: `Source/EchoAlchemist/Private/PCG/AttributeGenerator.cpp`<br>代码: `Source/EchoAlchemist/Public/PCG/MonsterAttributes.h`<br>文档: `Docs/CoreModules/PaletteGenerator.md`<br>文档: `Docs/CoreModules/ShapeGenerator.md`<br>文档: `Docs/CoreModules/ProceduralAnimator.md`<br>文档: `Docs/CoreModules/WFCAssembler.md`<br>文档: `Docs/CoreModules/AttributeGenerator.md`<br>文档: `Docs/Examples/BP_MonsterGenerator_*.md`（3个文档）<br>测试: `Source/EchoAlchemist/Tests/PCG/`（5个测试文件） | ✅ PCG核心算法已实现<br>✅ 属性系统已实现（2025-12-08）<br>✅ 单元测试已补充（2025-12-08）<br>✅ 完美遵循四层架构<br>❌ 视觉层待用户实现（精灵资源、Flipbook动画、调色板材质） |
| VOI-18 | 打砖块核心战斗系统 | In Progress | Urgent | TDD: `docs/tech/combat_system_tdd.md`<br>旧TDD: `docs/TDD_VOI-18_Combat_System.md` | 代码: `Source/EchoAlchemist/Public/Physics/MarblePhysicsSystem.h`<br>代码: `Source/EchoAlchemist/Private/Physics/MarblePhysicsSystem.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/CollisionManager.h`<br>代码: `Source/EchoAlchemist/Private/Physics/CollisionManager.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/MarbleActorPool.h`<br>代码: `Source/EchoAlchemist/Private/Physics/MarbleActorPool.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/MarbleActor.h`<br>代码: `Source/EchoAlchemist/Private/Physics/MarbleActor.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/SpatialGrid.h`<br>代码: `Source/EchoAlchemist/Private/Physics/SpatialGrid.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/PhysicsSceneConfig.h`<br>代码: `Source/EchoAlchemist/Private/Physics/PhysicsSceneConfig.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/SpecialEffectSystem.h`<br>代码: `Source/EchoAlchemist/Private/Physics/SpecialEffectSystem.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/SpecialEffectData.h`<br>代码: `Source/EchoAlchemist/Public/Physics/SpecialEffectBlueprintLibrary.h`<br>代码: `Source/EchoAlchemist/Private/Physics/SpecialEffectBlueprintLibrary.cpp`<br>代码: `Source/EchoAlchemist/Public/Combat/`（19个文件）<br>代码: `Source/EchoAlchemist/Private/Combat/`（8个文件）<br>文档: `Docs/CoreModules/PhysicsSystem.md`<br>文档: `Docs/CoreModules/HybridPhysicsSystem.md`<br>文档: `Docs/CoreModules/CollisionSystem.md`<br>文档: `Docs/CoreModules/SpecialEffectSystem.md`<br>测试: `Source/EchoAlchemist/Tests/Physics/`（5个测试文件） | ✅ TDD已补充（2025-11-30）<br>✅ 物理引擎已实现（弹珠物理、碰撞检测）<br>✅ 核心战斗系统已实现（2025-11-30）<br>✅ 数据层：5个数据结构<br>✅ 模拟层：伤害计算器、场景管理器（下落式+环形）、战斗管理器、敌人管理器<br>✅ 集成层：战斗物理集成器（碰撞检测、伤害处理）<br>✅ 事件系统：战斗事件系统<br>✅ 粘合层：战斗系统蓝图函数库<br>✅ 集成测试：完整的测试套件<br>✅ 完美遵循四层架构<br>✅ 物理系统与战斗系统已集成（2025-11-30）<br>✅ 特殊效果系统已实现（2025-12-08）<br>❌ 蓝图示例待创建 |
| VOI-26 | VOI-18.3: 实现混合物理系统 (Actor + Niagara) | In Progress | High | 无 | 代码: `Source/EchoAlchemist/Public/Physics/MarbleActor.h`<br>代码: `Source/EchoAlchemist/Private/Physics/MarbleActor.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/MarbleActorPool.h`<br>代码: `Source/EchoAlchemist/Private/Physics/MarbleActorPool.cpp`<br>文档: `Docs/CoreModules/HybridPhysicsSystem.md`<br>测试: `Source/EchoAlchemist/Tests/Physics/MarbleActorPoolTest.cpp` | ✅ AMarbleActor类已实现<br>✅ UMarbleActorPool对象池已实现<br>✅ 分级降级策略已设计<br>✅ 对象池管理已实现<br>❌ Niagara粒子系统集成待在蓝图中配置 |
| VOI-27 | VOI-18.4: 实现GDD特殊物理效果 | In Progress | High | 无 | 代码: `Source/EchoAlchemist/Public/Physics/SpecialEffectSystem.h`<br>代码: `Source/EchoAlchemist/Private/Physics/SpecialEffectSystem.cpp`<br>代码: `Source/EchoAlchemist/Public/Physics/SpecialEffectData.h`<br>代码: `Source/EchoAlchemist/Public/Physics/SpecialEffectBlueprintLibrary.h`<br>代码: `Source/EchoAlchemist/Private/Physics/SpecialEffectBlueprintLibrary.cpp`<br>文档: `Docs/CoreModules/SpecialEffectSystem.md`<br>测试: `Source/EchoAlchemist/Tests/Physics/SpecialEffectSystemTest.cpp` | ✅ 引力奇点效果已实现<br>✅ 虫洞传送效果已实现<br>✅ 弹珠分裂效果已实现<br>✅ 速度修改效果已实现<br>✅ 连锁触发机制已实现<br>✅ 蓝图函数库已实现<br>✅ 单元测试已完成<br>✅ 完美遵循四层架构 |

### 待实现的任务（Backlog）

| 任务ID | 任务标题 | 状态 | 优先级 | 协议仓库资源 | Game仓库资源 | 备注 |
|--------|----------|------|--------|--------------|--------------|------|
| VOI-12 | 炼金配方系统 - 魔药组合与配方库 | Todo | Urgent | TDD: `docs/tech/recipe_system_tdd.md` | 无 | ✅ TDD已创建（2025-11-30）<br>❌ 代码待实现 |
| VOI-13 | 教学与引导系统 - 基于AINPC的渐进式教学 | Backlog | Medium | TDD: 待创建 | 无 | 可在MVP后实现 |
| VOI-14 | 资源管理与经济系统 - 材料采集与消耗平衡 | Backlog | High | TDD: 待创建 | 无 | 经济系统基础 |
| VOI-15 | 铭刻解谜生成器 - 程序化生成填字谜题 | Backlog | Medium | TDD: 待创建 | 无 | 内容生成优化 |
| VOI-16 | 战斗反馈与打击感系统 - 弹珠物理与视觉反馈 | Backlog | High | TDD: 待创建 | 无 | 玩家体验增强 |
| VOI-19 | 地图探索与循环管理系统 - Roguelike元层框架 | Backlog | Urgent | TDD: 待创建 | 无 | 元层框架，整合所有系统 |

---

## 更新日志

| 日期 | 任务ID | 更新内容 | 更新者 |
|------|--------|----------|--------|
| 2025-11-29 | VOI-5 | 初始记录 | Manus AI - 项目策略师 |
| 2025-11-29 | VOI-6 | 初始记录，补充完整资源位置 | Manus AI - 项目策略师 |
| 2025-11-29 | VOI-11 | 初始记录，补充完整资源位置 | Manus AI - 项目策略师 |
| 2025-11-29 | VOI-18 | 初始记录，补充完整资源位置 | Manus AI - 项目策略师 |
| 2025-11-29 | VOI-12到VOI-16、VOI-19 | 初始记录，标记为待实现 | Manus AI - 项目策略师 |
| 2025-11-30 | VOI-12 | 创建TDD，更新状态为Todo | Manus AI - 自主开发者 |
| 2025-11-30 | VOI-18 | 补充完整TDD，更新状态为Todo | Manus AI - 自主开发者 |
| 2025-11-30 | VOI-18 | 实现核心战斗系统框架（数据层、伤害计算器、场景管理器、战斗管理器、事件系统、蓝图函数库），更新状态为In Progress | Manus AI - 自主开发者 |
| 2025-11-30 | VOI-18 | 实现敌人管理器和环形场景管理器，添加集成测试套件 | Manus AI - 自主开发者 |
| 2025-11-30 | VOI-18 | 实现物理系统与战斗系统的集成，创建战斗物理集成器 | Manus AI - 自主开发者 |
| 2025-12-08 | VOI-24 | 标记为已完成，物理系统核心框架已实现 | Manus AI - 自主开发者 |
| 2025-12-08 | VOI-25 | 标记为已完成，碰撞检测系统已实现 | Manus AI - 自主开发者 |
| 2025-12-08 | VOI-26 | 标记为进行中，混合物理系统C++核心已完成，Niagara集成待蓝图配置 | Manus AI - 自主开发者 |
| 2025-12-08 | VOI-27 | 实现特殊效果系统（引力奇点、虫洞传送、弹珠分裂、速度修改、连锁触发），标记为进行中 | Manus AI - 自主开发者 |
| 2025-12-08 | VOI-27 | 更新Linear状态为Done，代码已合并到main分支 | Manus AI - 自主开发者 |
| 2025-12-08 | VOI-11 | 补充属性生成器单元测试，更新任务资源映射表 | Manus AI - 自主开发者 |

---

## 附录：资源路径说明

### 协议仓库（ue4-human-ai-collab）

```
ue4-human-ai-collab/
├── docs/
│   ├── tech/                          # TDD存放目录
│   │   ├── world_morphing_system_tdd.md
│   │   ├── monster_generation_system_tdd.md
│   │   └── simulator/                 # 模拟器代码
│   ├── mhp/                           # MHP存放目录
│   │   ├── VOI-5_git_setup.md
│   │   └── VOI-6_world_morphing.md
│   └── task_resource_mapping.md       # 本文件
```

### Game仓库（ue4-human-ai-collab-game）

```
ue4-human-ai-collab-game/
├── Source/
│   └── EchoAlchemist/
│       ├── Public/                    # 头文件
│       │   ├── WorldMorphing/
│       │   ├── PCG/
│       │   └── Physics/
│       ├── Private/                   # 实现文件
│       │   ├── WorldMorphing/
│       │   ├── PCG/
│       │   └── Physics/
│       └── Tests/                     # 测试文件
│           ├── PCG/
│           └── Physics/
├── Docs/
│   ├── CoreModules/                   # 核心模块文档
│   │   ├── WorldMorphingSystem*.md
│   │   ├── PhysicsSystem.md
│   │   └── ...
│   └── Examples/                      # 示例文档
│       └── BP_MonsterGenerator_*.md
└── Content/
    └── Blueprints/                    # 蓝图资源
```

---

## 使用指南

### 如何查找任务的所有资源？

1. 在索引表中找到任务ID对应的行
2. 查看"协议仓库资源"列，找到TDD、MHP的路径
3. 查看"Game仓库资源"列，找到代码、文档、测试的路径
4. 根据路径在对应的仓库中查找资源

### 如何更新索引表？

1. 在创建新任务时，立即在索引表中添加一行记录
2. 在创建TDD、MHP、代码、文档、测试时，立即更新对应的列
3. 在任务状态变更时，更新"状态"列
4. 在对话结束前，确保索引表已更新
5. 提交索引表到Git仓库

### 如何验证索引表的完整性？

使用自检流程规范（`docs/self_check_specification.md`）中的检查清单，逐项检查：

- [ ] 任务ID是否记录？
- [ ] 任务标题是否记录？
- [ ] 任务状态是否正确？
- [ ] 协议仓库资源路径是否记录？
- [ ] Game仓库资源路径是否记录？
- [ ] 备注是否清晰？

---

**参考资料**:
1. [自检流程规范](self_check_specification.md)
2. [TDD规范](tdd_specification.md)
3. [人机协同开发规范](collaboration_specification.md)
4. [两阶段工作流规范](workflow_specification.md)
