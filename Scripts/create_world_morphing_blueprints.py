"""
自动创建WorldMorphingSystem蓝图资产

此脚本在UE4编辑器内运行，自动创建测试和可视化所需的蓝图。

运行方法：
1. 在UE4编辑器中打开项目
2. 在Output Log中输入：py "Scripts/create_world_morphing_blueprints.py"

创建的蓝图：
- BP_WorldMorphingVisualizer - 可视化Actor
- BP_WorldMorphingController - 控制器Actor
- WBP_WorldMorphingUI - UI Widget
"""

import unreal

def log_section(title):
    """打印章节标题"""
    unreal.log("=" * 60)
    unreal.log(f"  {title}")
    unreal.log("=" * 60)

def create_visualizer_actor():
    """创建可视化Actor蓝图"""
    log_section("创建 BP_WorldMorphingVisualizer")
    
    try:
        # 设置资产路径
        asset_path = "/Game/Blueprints/Systems/WorldMorphing"
        asset_name = "BP_WorldMorphingVisualizer"
        full_path = f"{asset_path}/{asset_name}"
        
        # 检查是否已存在
        if unreal.EditorAssetLibrary.does_asset_exist(full_path):
            unreal.log_warning(f"⚠️  资产已存在: {full_path}")
            unreal.log("正在删除旧资产...")
            unreal.EditorAssetLibrary.delete_asset(full_path)
        
        # 创建蓝图
        factory = unreal.BlueprintFactory()
        factory.set_editor_property("parent_class", unreal.Actor)
        
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        blueprint = asset_tools.create_asset(
            asset_name,
            asset_path,
            unreal.Blueprint,
            factory
        )
        
        if blueprint:
            unreal.log(f"✅ 蓝图已创建: {full_path}")
            
            # 添加组件说明注释
            unreal.log("提示: 请手动添加以下组件:")
            unreal.log("  1. Instanced Static Mesh Component (用于渲染网格)")
            unreal.log("  2. Scene Component (作为根组件)")
            
            # 保存资产
            unreal.EditorAssetLibrary.save_asset(full_path)
            return blueprint
        else:
            unreal.log_error("❌ 创建蓝图失败")
            return None
            
    except Exception as e:
        unreal.log_error(f"❌ 创建可视化Actor失败: {str(e)}")
        return None

def create_controller_actor():
    """创建控制器Actor蓝图"""
    log_section("创建 BP_WorldMorphingController")
    
    try:
        asset_path = "/Game/Blueprints/Systems/WorldMorphing"
        asset_name = "BP_WorldMorphingController"
        full_path = f"{asset_path}/{asset_name}"
        
        # 检查是否已存在
        if unreal.EditorAssetLibrary.does_asset_exist(full_path):
            unreal.log_warning(f"⚠️  资产已存在: {full_path}")
            unreal.log("正在删除旧资产...")
            unreal.EditorAssetLibrary.delete_asset(full_path)
        
        # 创建蓝图
        factory = unreal.BlueprintFactory()
        factory.set_editor_property("parent_class", unreal.Actor)
        
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        blueprint = asset_tools.create_asset(
            asset_name,
            asset_path,
            unreal.Blueprint,
            factory
        )
        
        if blueprint:
            unreal.log(f"✅ 蓝图已创建: {full_path}")
            
            unreal.log("提示: 在Event Graph中添加以下逻辑:")
            unreal.log("  1. Event BeginPlay -> Initialize World")
            unreal.log("  2. Event Tick -> Tick Simulation")
            unreal.log("  3. 添加变量: Grid Width, Grid Height")
            
            unreal.EditorAssetLibrary.save_asset(full_path)
            return blueprint
        else:
            unreal.log_error("❌ 创建蓝图失败")
            return None
            
    except Exception as e:
        unreal.log_error(f"❌ 创建控制器Actor失败: {str(e)}")
        return None

def create_ui_widget():
    """创建UI Widget蓝图"""
    log_section("创建 WBP_WorldMorphingUI")
    
    try:
        asset_path = "/Game/UI/WorldMorphing"
        asset_name = "WBP_WorldMorphingUI"
        full_path = f"{asset_path}/{asset_name}"
        
        # 检查是否已存在
        if unreal.EditorAssetLibrary.does_asset_exist(full_path):
            unreal.log_warning(f"⚠️  资产已存在: {full_path}")
            unreal.log("正在删除旧资产...")
            unreal.EditorAssetLibrary.delete_asset(full_path)
        
        # 创建Widget蓝图
        factory = unreal.WidgetBlueprintFactory()
        
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        widget_bp = asset_tools.create_asset(
            asset_name,
            asset_path,
            unreal.WidgetBlueprint,
            factory
        )
        
        if widget_bp:
            unreal.log(f"✅ Widget蓝图已创建: {full_path}")
            
            unreal.log("提示: 在Designer中添加以下UI元素:")
            unreal.log("  1. Text Block - 显示时间步和周期数")
            unreal.log("  2. Slider - 调整参数")
            unreal.log("  3. Button - 重置世界")
            unreal.log("  4. Canvas Panel - 显示统计信息")
            
            unreal.EditorAssetLibrary.save_asset(full_path)
            return widget_bp
        else:
            unreal.log_error("❌ 创建Widget蓝图失败")
            return None
            
    except Exception as e:
        unreal.log_error(f"❌ 创建UI Widget失败: {str(e)}")
        return None

def create_material():
    """创建可视化材质"""
    log_section("创建 M_WorldMorphingCell")
    
    try:
        asset_path = "/Game/Materials/WorldMorphing"
        asset_name = "M_WorldMorphingCell"
        full_path = f"{asset_path}/{asset_name}"
        
        # 检查是否已存在
        if unreal.EditorAssetLibrary.does_asset_exist(full_path):
            unreal.log_warning(f"⚠️  材质已存在: {full_path}")
            return unreal.load_asset(full_path)
        
        # 创建材质
        factory = unreal.MaterialFactoryNew()
        
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        material = asset_tools.create_asset(
            asset_name,
            asset_path,
            unreal.Material,
            factory
        )
        
        if material:
            unreal.log(f"✅ 材质已创建: {full_path}")
            
            unreal.log("提示: 在材质编辑器中:")
            unreal.log("  1. 添加 Vertex Color 节点连接到 Base Color")
            unreal.log("  2. 设置 Shading Model 为 Unlit")
            unreal.log("  3. 启用 Two Sided")
            
            unreal.EditorAssetLibrary.save_asset(full_path)
            return material
        else:
            unreal.log_error("❌ 创建材质失败")
            return None
            
    except Exception as e:
        unreal.log_error(f"❌ 创建材质失败: {str(e)}")
        return None

def create_test_level():
    """创建测试关卡"""
    log_section("创建测试关卡")
    
    try:
        level_path = "/Game/Maps/TestMaps"
        level_name = "WorldMorphingTest"
        full_path = f"{level_path}/{level_name}"
        
        # 检查是否已存在
        if unreal.EditorAssetLibrary.does_asset_exist(full_path):
            unreal.log_warning(f"⚠️  关卡已存在: {full_path}")
            unreal.log("跳过创建")
            return True
        
        # 创建新关卡
        unreal.EditorLevelLibrary.new_level(full_path)
        unreal.log(f"✅ 测试关卡已创建: {full_path}")
        
        unreal.log("提示: 在关卡中放置:")
        unreal.log("  1. BP_WorldMorphingController")
        unreal.log("  2. BP_WorldMorphingVisualizer")
        unreal.log("  3. 相机和光源")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 创建测试关卡失败: {str(e)}")
        return False

def create_blueprint_function_library_wrapper():
    """创建蓝图函数库包装器说明"""
    log_section("蓝图函数库使用说明")
    
    unreal.log("WorldMorphingBlueprintLibrary 已在C++中实现")
    unreal.log("可在蓝图中直接使用以下节点:")
    unreal.log("")
    unreal.log("初始化:")
    unreal.log("  - Initialize World")
    unreal.log("  - Make Default Params")
    unreal.log("")
    unreal.log("更新:")
    unreal.log("  - Tick Simulation")
    unreal.log("")
    unreal.log("查询:")
    unreal.log("  - Get Cell At")
    unreal.log("  - Get Grid Size")
    unreal.log("  - Get Time Step")
    unreal.log("  - Get Cycle Count")
    unreal.log("")
    unreal.log("参数:")
    unreal.log("  - Set Simulation Params")
    unreal.log("  - Get Simulation Params")

def main():
    """主函数"""
    unreal.log("=" * 80)
    unreal.log("  WorldMorphingSystem 蓝图资产创建器")
    unreal.log("=" * 80)
    unreal.log("")
    
    # 创建目录结构
    unreal.log("正在创建目录结构...")
    directories = [
        "/Game/Blueprints/Systems/WorldMorphing",
        "/Game/UI/WorldMorphing",
        "/Game/Materials/WorldMorphing",
        "/Game/Maps/TestMaps"
    ]
    
    for directory in directories:
        if not unreal.EditorAssetLibrary.does_directory_exist(directory):
            unreal.EditorAssetLibrary.make_directory(directory)
            unreal.log(f"✅ 目录已创建: {directory}")
    
    unreal.log("")
    
    # 创建资产
    results = []
    
    # 1. 创建可视化Actor
    visualizer = create_visualizer_actor()
    results.append(("BP_WorldMorphingVisualizer", visualizer is not None))
    unreal.log("")
    
    # 2. 创建控制器Actor
    controller = create_controller_actor()
    results.append(("BP_WorldMorphingController", controller is not None))
    unreal.log("")
    
    # 3. 创建UI Widget
    ui_widget = create_ui_widget()
    results.append(("WBP_WorldMorphingUI", ui_widget is not None))
    unreal.log("")
    
    # 4. 创建材质
    material = create_material()
    results.append(("M_WorldMorphingCell", material is not None))
    unreal.log("")
    
    # 5. 创建测试关卡
    level = create_test_level()
    results.append(("WorldMorphingTest Level", level))
    unreal.log("")
    
    # 6. 显示函数库说明
    create_blueprint_function_library_wrapper()
    unreal.log("")
    
    # 打印结果摘要
    log_section("创建结果摘要")
    
    for name, success in results:
        status = "✅ 成功" if success else "❌ 失败"
        unreal.log(f"  {status} - {name}")
    
    passed = sum(1 for _, success in results if success)
    total = len(results)
    
    unreal.log("")
    unreal.log(f"总计: {passed}/{total} 资产创建成功")
    
    if passed == total:
        unreal.log("🎉 所有资产创建完成！")
    else:
        unreal.log_warning(f"⚠️  有 {total - passed} 个资产创建失败")
    
    unreal.log("")
    log_section("下一步")
    unreal.log("1. 打开 BP_WorldMorphingController，在Event Graph中添加逻辑")
    unreal.log("2. 打开 BP_WorldMorphingVisualizer，添加Instanced Static Mesh组件")
    unreal.log("3. 打开 WBP_WorldMorphingUI，设计UI界面")
    unreal.log("4. 打开 WorldMorphingTest 关卡，放置Actor并测试")
    unreal.log("")
    unreal.log("=" * 80)

if __name__ == "__main__":
    main()
