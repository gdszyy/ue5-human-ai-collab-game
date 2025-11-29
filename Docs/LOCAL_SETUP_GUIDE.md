# 《回声炼金师》本地环境配置指南

## 前置要求

### 必需软件

1. **Unreal Engine 4.27.2**
   - 确保已安装 UE4 4.27.2 版本
   - 如果没有安装，请从 Epic Games Launcher 下载安装

2. **Git 和 Git LFS**
   - Git 版本 >= 2.30
   - Git LFS 已安装并初始化

3. **Visual Studio 2019 或 2022**（用于 C++ 编译）
   - 工作负载：使用 C++ 的游戏开发
   - 组件：Windows 10 SDK

### 可选软件

- **Python 3.7+**（用于运行外部测试脚本）

## 步骤一：克隆项目

### 1. 打开命令行工具

在您希望存放项目的目录下打开命令行（CMD 或 PowerShell）。

### 2. 克隆仓库

```bash
git clone https://github.com/gdszyy/ue4-human-ai-collab-game.git
cd ue4-human-ai-collab-game
```

### 3. 验证 Git LFS

确保 Git LFS 正常工作：

```bash
git lfs install
git lfs pull
```

## 步骤二：生成项目文件

### 1. 右键点击 `EchoAlchemist.uproject`

在文件资源管理器中，右键点击 `EchoAlchemist.uproject` 文件。

### 2. 选择 "Generate Visual Studio project files"

这将生成 Visual Studio 解决方案文件（`.sln`）。

**注意**：如果没有看到此选项，说明 UE4 4.27 没有正确关联 `.uproject` 文件。请：
- 打开 Epic Games Launcher
- 进入 "Library" -> "Engine Versions"
- 点击 UE4 4.27 旁边的 "..." 按钮
- 选择 "Set as default"

### 3. 等待生成完成

生成过程可能需要几分钟。

## 步骤三：编译 C++ 代码

### 方法一：通过 Visual Studio（推荐）

1. 打开生成的 `EchoAlchemist.sln` 文件
2. 在 Visual Studio 中，选择 "Development Editor" 配置
3. 右键点击 "EchoAlchemist" 项目，选择 "Build"
4. 等待编译完成

### 方法二：通过 UE4 编辑器

1. 双击 `EchoAlchemist.uproject` 打开项目
2. UE4 会提示需要编译，点击 "Yes" 确认
3. 等待编译完成

## 步骤四：打开项目

### 1. 双击 `EchoAlchemist.uproject`

项目将在 UE4 编辑器中打开。

### 2. 首次打开可能需要较长时间

UE4 需要：
- 编译着色器
- 加载资产
- 初始化插件

请耐心等待。

## 步骤五：验证插件启用

### 1. 打开插件管理器

在 UE4 编辑器中，选择 `Edit` -> `Plugins`。

### 2. 验证以下插件已启用

- **Remote Control**（在 "Other" 分类下）
- **Python Script Plugin**（在 "Scripting" 分类下）
- **Editor Scripting Utilities**（在 "Scripting" 分类下）

如果未启用，请勾选并重启编辑器。

## 步骤六：配置 Git 源码管理（可选）

### 1. 打开源码管理设置

在 UE4 编辑器中，选择 `Edit` -> `Editor Preferences` -> `Source Control`。

### 2. 选择 Git 提供方

- **Provider**: Git (beta)
- **Git Path**: 您的 Git 可执行文件路径（通常是 `C:/Program Files/Git/bin/git.exe`）

### 3. 接受设置

点击 "Accept Settings"。

### 4. 验证连接

在编辑器右下角，您应该能看到 Git 图标，显示当前分支。

## 步骤七：测试 Remote Control API

### 1. 确保 UE4 编辑器已打开项目

### 2. 打开浏览器

访问 `http://localhost:30010/remote/info`

### 3. 验证响应

如果看到 JSON 响应，说明 Remote Control API 已成功启动。

示例响应：

```json
{
  "HttpRoutes": [
    {
      "Path": "/remote/info",
      "Verb": "Get",
      "Description": "Get information about different routes available on this API."
    },
    ...
  ]
}
```

### 4. 运行 Python 测试脚本（可选）

如果您安装了 Python，可以运行测试脚本：

```bash
cd Scripts
python test_remote_control.py
```

## 步骤八：测试 Python 编辑器脚本（可选）

### 1. 在 UE4 编辑器中打开 Output Log

选择 `Window` -> `Developer Tools` -> `Output Log`。

### 2. 执行 Python 脚本

在 Output Log 的 Cmd 输入框中输入：

```
py Scripts/ue4_editor_example.py
```

按 Enter 执行。

### 3. 查看输出

您应该能在 Output Log 中看到脚本的执行结果。

## 常见问题

### Q1: 编译失败，提示找不到 UE4 头文件

**解决方案**：
1. 确保 UE4 4.27.2 已正确安装
2. 重新生成项目文件（右键 `.uproject` -> Generate Visual Studio project files）
3. 在 Visual Studio 中清理并重新生成解决方案

### Q2: 打开项目时提示模块缺失

**解决方案**：
点击 "Yes" 让 UE4 自动编译缺失的模块。

### Q3: Remote Control API 无法访问

**解决方案**：
1. 确保 Remote Control 插件已启用
2. 检查 `Config/DefaultEngine.ini` 中的配置
3. 重启 UE4 编辑器
4. 检查防火墙是否阻止了 30010 端口

### Q4: Git LFS 文件下载失败

**解决方案**：
```bash
git lfs install
git lfs fetch --all
git lfs pull
```

### Q5: 项目打开后是空白场景

**解决方案**：
这是正常的，因为我们还没有创建默认关卡。您可以：
1. 创建一个新的 Level：`File` -> `New Level`
2. 保存到 `Content/Maps/DefaultLevel`

## Git 工作流

### 拉取最新更新

```bash
git pull origin main
```

### 提交本地修改

```bash
git add .
git commit -m "描述您的修改"
git push origin main
```

**注意**：由于 `main` 分支有保护规则，您需要：
1. 创建新分支：`git checkout -b feature/your-feature-name`
2. 推送分支：`git push origin feature/your-feature-name`
3. 在 GitHub 上创建 Pull Request

## 人机协同工作流

### Manus 更新项目

1. Manus 在沙盒中修改项目文件
2. Manus 创建 Pull Request
3. 您在 GitHub 上审核并合并 PR
4. 在本地执行 `git pull` 拉取更新
5. UE4 编辑器会自动检测文件变化并重新加载

### 您更新项目

1. 在 UE4 中进行开发
2. 通过 Git 提交并推送
3. Manus 拉取更新以了解项目状态

## 下一步

- 阅读 [项目结构说明](PROJECT_STRUCTURE.md)
- 阅读 [人机协同开发规范](../../ue4-human-ai-collab/docs/collaboration_specification.md)
- 开始开发！

## 需要帮助？

如果遇到问题，请：
1. 查看本文档的"常见问题"部分
2. 在 GitHub 仓库中提 Issue
3. 在 Linear 任务中添加评论
