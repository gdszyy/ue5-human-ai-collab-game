# 《回声炼金师》(Echo Alchemist)

基于人机协同开发模式的 UE4 4.27 游戏项目。

## 项目简介

《回声炼金师》是一款融合炼金术与弹珠打砖块玩法的策略游戏。本项目采用创新的人机协同开发模式，由 AI（Manus）和人类开发者共同完成。

## 快速开始

### 环境要求

- **Unreal Engine**: 4.27.2
- **Visual Studio**: 2019 或 2022
- **Git**: >= 2.30（已安装 Git LFS）
- **操作系统**: Windows 10/11

### 克隆项目

```bash
git clone https://github.com/gdszyy/ue4-human-ai-collab-game.git
cd ue4-human-ai-collab-game
git lfs install
git lfs pull
```

### 打开项目

1. 右键点击 `EchoAlchemist.uproject`
2. 选择 "Generate Visual Studio project files"
3. 双击 `EchoAlchemist.uproject` 打开项目
4. 等待 UE4 编译 C++ 代码

详细步骤请参考 [本地环境配置指南](Docs/LOCAL_SETUP_GUIDE.md)。

## 项目特性

### 已启用的插件

- **Remote Control**: 支持通过 HTTP/WebSocket 远程控制 UE4
- **Python Script Plugin**: 支持 Python 编辑器自动化
- **Editor Scripting Utilities**: 编辑器脚本工具

### Remote Control API

- **端口**: 30010
- **测试**: 打开 UE4 后访问 `http://localhost:30010/remote/info`
- **文档**: [Remote Control API 官方文档](https://dev.epicgames.com/documentation/en-us/unreal-engine/remote-control-for-unreal-engine)

## 项目结构

```
ue4-human-ai-collab-game/
├── EchoAlchemist.uproject    # UE4 项目文件
├── Config/                    # 项目配置
├── Content/                   # 游戏资产
│   ├── Blueprints/           # 蓝图
│   ├── Maps/                 # 关卡
│   ├── Characters/           # 角色
│   └── Systems/              # 游戏系统
├── Source/                    # C++ 源代码
├── Scripts/                   # Python 脚本
├── Docs/                      # 项目文档
├── .gitignore                # Git 忽略规则
├── .gitattributes            # Git LFS 配置
└── README.md                 # 本文件
```

详细说明请参考 [项目结构文档](Docs/PROJECT_STRUCTURE.md)。

## 开发规范

### Git 工作流

- 使用 Git Flow 分支策略
- `main` 分支受保护，需要通过 Pull Request 合并
- 分支命名：`feature/`, `bugfix/`, `hotfix/`

### 提交规范

使用 Conventional Commits：

```
feat: 新功能
fix: 修复 bug
docs: 文档更新
style: 代码格式
refactor: 重构
test: 测试
chore: 构建/工具
```

### 人机协同流程

1. **Manus → 本地**：Manus 修改项目 → 推送到 Git → 您本地 `git pull`
2. **本地 → Manus**：您本地开发 → 推送到 Git → Manus 拉取更新

## 相关文档

- [本地环境配置指南](Docs/LOCAL_SETUP_GUIDE.md)
- [项目结构说明](Docs/PROJECT_STRUCTURE.md)
- [人机协同开发规范](https://github.com/gdszyy/ue4-human-ai-collab/blob/master/docs/collaboration_specification.md)
- [两阶段工作流规范](https://github.com/gdszyy/ue4-human-ai-collab/blob/master/docs/workflow_specification.md)

## 项目管理

- **任务管理**: [Linear](https://linear.app)
- **代码仓库**: [GitHub](https://github.com/gdszyy/ue4-human-ai-collab-game)
- **游戏设计文档**: [Notion](https://www.notion.so/2b8bacf5f26781c0b16ccec48d9750cc)

## 许可证

Copyright 2025 Voidzyy. All Rights Reserved.
