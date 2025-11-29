# EchoAlchemist 项目交接文档

## 📋 文档概述

本文档为 EchoAlchemist 项目的交接文档，包含项目的关键信息、开发规范、常见问题和疑难杂症解决方案。

**最后更新**：2025-11-30  
**维护者**：EchoAlchemist 开发团队

---

## 📂 项目基本信息

### 项目简介

**项目名称**：EchoAlchemist  
**项目类型**：UE4 游戏项目  
**引擎版本**：Unreal Engine 4.27  
**开发语言**：C++, Blueprint  
**版本控制**：Git (GitHub)

### 项目结构

详见：[PROJECT_STRUCTURE.md](./PROJECT_STRUCTURE.md)

主要目录：
```
ue4-human-ai-collab-game/
├── Source/EchoAlchemist/          # C++ 源代码
│   ├── Public/                    # 公共头文件
│   │   └── Physics/               # 物理系统
│   ├── Private/                   # 私有实现文件
│   │   └── Physics/               # 物理系统实现
│   └── Tests/                     # 自动化测试
│       └── Physics/               # 物理系统测试
├── Content/                       # 蓝图和资源
├── Docs/                          # 项目文档
│   ├── CoreModules/               # 核心模块文档
│   ├── Troubleshooting/           # 疑难杂症知识库 ⭐
│   └── Examples/                  # 示例代码
└── Config/                        # 配置文件
```

---

## 🔧 开发环境设置

### 必需软件

1. **Unreal Engine 4.27**
2. **Visual Studio 2019/2022**（Windows）或 **Xcode**（macOS）
3. **Git**

### 环境配置

详见：[LOCAL_SETUP_GUIDE.md](./LOCAL_SETUP_GUIDE.md)

快速开始：
```bash
# 1. 克隆仓库
git clone https://github.com/gdszyy/ue4-human-ai-collab-game.git

# 2. 生成项目文件
右键 .uproject 文件 -> Generate Visual Studio project files

# 3. 编译项目
打开 .sln 文件 -> Build Solution (Ctrl+Shift+B)

# 4. 启动编辑器
双击 .uproject 文件
```

---

## 📚 核心模块文档

### 物理系统

详见：[CoreModules/PhysicsSystem.md](./CoreModules/PhysicsSystem.md)

核心类：
- `UMarblePhysicsSystem`：魔力露珠物理系统
- `UCollisionManager`：碰撞检测管理器
- `UMarbleActorPool`：魔力露珠对象池

### 测试框架

详见：[CoreModules/TestingGuide.md](./CoreModules/TestingGuide.md)

测试位置：
- 单元测试：`Source/EchoAlchemist/Tests/Physics/`
- 运行测试：Window -> Test Automation

---

## 🚨 疑难杂症知识库 ⭐⭐⭐

**这是最重要的部分！**

在开发过程中遇到的各种非常规问题和解决方案都记录在这里。

### 📍 知识库位置

**文档路径**：[Docs/Troubleshooting/](./Troubleshooting/)

**索引目录**：[Docs/Troubleshooting/README.md](./Troubleshooting/README.md)

### 📋 已知问题列表

| 编号 | 问题 | 严重性 | 修复难度 | 文档链接 |
|------|------|--------|----------|----------|
| 001 | 自动化测试无法执行（显示"0次测试"） | 🔴 高 | 🟢 低 | [001-AutomationTestNotRunning.md](./Troubleshooting/001-AutomationTestNotRunning.md) |

### 🔍 如何使用疑难杂症知识库

#### 遇到问题时

1. 打开 [Troubleshooting/README.md](./Troubleshooting/README.md)
2. 在索引目录中查找相关问题
3. 点击链接查看详细解决方案
4. 按照文档中的步骤操作

#### 添加新问题时

1. 在 `Docs/Troubleshooting/` 文件夹中创建新文档
2. 文件命名：`XXX-ProblemName.md`（XXX 为三位数字编号）
3. 使用统一的文档模板（见 [Troubleshooting/README.md](./Troubleshooting/README.md)）
4. 更新 `Troubleshooting/README.md` 的索引目录
5. 更新本交接文档的问题列表
6. 提交到 Git 仓库

#### 文档质量要求

- ✅ 问题描述清晰，包含具体现象
- ✅ 根本原因分析透彻，有理论依据
- ✅ 修复方案可操作，步骤详细
- ✅ 包含代码示例和命令
- ✅ 标注严重性、修复难度、影响范围

---

## 🎯 开发规范

### 代码规范

1. **命名规范**：
   - 类名：`UMyClass`（U 前缀）
   - 结构体：`FMyStruct`（F 前缀）
   - 枚举：`EMyEnum`（E 前缀）
   - 接口：`IMyInterface`（I 前缀）

2. **注释规范**：
   - 所有公共 API 必须有文档注释
   - 使用 `/** */` 格式
   - 包含参数说明和返回值说明

3. **测试规范**：
   - 所有新功能必须有单元测试
   - 测试文件命名：`[ClassName]Test.cpp`
   - 测试必须有 `WITH_DEV_AUTOMATION_TESTS` 宏保护
   - 使用 `EditorContext` 而不是 `ApplicationContextMask`

### Git 工作流

1. **分支策略**：
   - `main`：主分支，保持稳定
   - `develop`：开发分支
   - `feature/*`：功能分支
   - `bugfix/*`：修复分支

2. **提交规范**：
   ```
   <type>: <subject>
   
   <body>
   
   <footer>
   ```
   
   类型（type）：
   - `feat`：新功能
   - `fix`：修复 bug
   - `docs`：文档更新
   - `test`：测试相关
   - `refactor`：重构
   - `style`：代码格式

3. **提交示例**：
   ```
   fix: Change test flags from ApplicationContextMask to EditorContext
   
   - Replace ApplicationContextMask with EditorContext for all physics tests
   - This fixes the issue where tests show up but don't execute
   
   Affected files:
   - PhysicsSystemIntegrationTest.cpp (4 tests)
   - CollisionManagerTest.cpp (8 tests)
   ```

---

## 🐛 常见问题 FAQ

### Q1: 测试在列表中显示但不执行？

**A**: 这是最常见的问题！

**原因**：使用了错误的 `EAutomationTestFlags::ApplicationContextMask`

**解决方案**：查看 [Troubleshooting/001-AutomationTestNotRunning.md](./Troubleshooting/001-AutomationTestNotRunning.md)

**快速修复**：
```cpp
// 将这个
EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter

// 改为这个
EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter
```

### Q2: 编译错误：找不到 AutomationController 模块？

**A**: Build.cs 配置问题

**解决方案**：
```csharp
// 在 YourModule.Build.cs 中添加条件判断
if (Target.bBuildDeveloperTools || 
    (Target.Configuration != UnrealTargetConfiguration.Shipping && 
     Target.Configuration != UnrealTargetConfiguration.Test))
{
    PrivateDependencyModuleNames.Add("AutomationController");
}
```

### Q3: 如何运行测试？

**A**: 
1. 打开 UE4 编辑器
2. Window -> Test Automation
3. 搜索测试名称（例如：`EchoAlchemist.Physics`）
4. 选择测试
5. 点击 "Run Tests"

### Q4: 测试失败了怎么办？

**A**: 
1. 查看 Output Log 中的错误信息
2. 检查测试代码的断言条件
3. 使用 `UE_LOG` 添加调试日志
4. 如果是物理测试，检查时间步长和模拟帧数

---

## 📞 联系方式

### 项目维护者

- **GitHub**：[gdszyy](https://github.com/gdszyy)
- **仓库**：[ue4-human-ai-collab-game](https://github.com/gdszyy/ue4-human-ai-collab-game)

### 获取帮助

1. **查看文档**：先查看 `Docs/` 目录下的相关文档
2. **查看疑难杂症**：查看 `Docs/Troubleshooting/` 知识库
3. **提交 Issue**：在 GitHub 上提交 Issue
4. **联系维护者**：通过 GitHub 联系

---

## 📝 重要提醒

### ⚠️ 必读文档

在开始开发前，**必须**阅读以下文档：

1. ✅ [LOCAL_SETUP_GUIDE.md](./LOCAL_SETUP_GUIDE.md) - 环境设置
2. ✅ [PROJECT_STRUCTURE.md](./PROJECT_STRUCTURE.md) - 项目结构
3. ✅ [Troubleshooting/README.md](./Troubleshooting/README.md) - 疑难杂症索引 ⭐
4. ✅ [CoreModules/PhysicsSystem.md](./CoreModules/PhysicsSystem.md) - 物理系统
5. ✅ [CoreModules/TestingGuide.md](./CoreModules/TestingGuide.md) - 测试指南

### ⚠️ 开发注意事项

1. **不要使用 ApplicationContextMask**
   - 这是导致测试不执行的主要原因
   - 使用 `EditorContext` 替代

2. **测试代码必须有宏保护**
   - 使用 `#if WITH_DEV_AUTOMATION_TESTS`
   - 这是 UE4 的标准做法

3. **遇到问题先查疑难杂症知识库**
   - 很多问题已经有解决方案
   - 避免重复踩坑

4. **新问题要记录到知识库**
   - 帮助后来的开发者
   - 建立项目知识积累

---

## 🔄 文档更新记录

| 日期 | 更新内容 | 更新人 |
|------|----------|--------|
| 2025-11-30 | 创建交接文档，添加疑难杂症知识库 | Manus AI |
| 2025-11-30 | 添加测试不执行问题的解决方案 | Manus AI |

---

## 📌 快速链接

### 文档

- [项目结构](./PROJECT_STRUCTURE.md)
- [环境设置](./LOCAL_SETUP_GUIDE.md)
- [疑难杂症知识库](./Troubleshooting/README.md) ⭐
- [物理系统文档](./CoreModules/PhysicsSystem.md)
- [测试指南](./CoreModules/TestingGuide.md)

### 外部资源

- [UE4 官方文档](https://docs.unrealengine.com/4.27/)
- [UE4 C++ API Reference](https://docs.unrealengine.com/4.27/en-US/API/)
- [UE4 Automation Testing](https://docs.unrealengine.com/4.27/en-US/TestingAndOptimization/Automation/)

---

**欢迎加入 EchoAlchemist 项目！**

如有任何问题，请先查看文档，特别是 [疑难杂症知识库](./Troubleshooting/README.md)。
