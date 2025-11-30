"""
WorldMorphingSystem 测试脚本

此脚本需要在 UE4 编辑器内运行，用于测试世界变迁系统的核心功能。

运行方法：
1. 在 UE4 编辑器中打开项目
2. 打开 Window -> Developer Tools -> Output Log
3. 在 Output Log 的 Cmd 输入框中输入：py "Scripts/test_world_morphing.py"
4. 或者在编辑器菜单中选择 File -> Execute Python Script，然后选择此文件

测试内容：
- 初始化世界网格
- 执行模拟更新
- 读取单元格状态
- 参数调整测试
- 性能基准测试
"""

import unreal
import time

def log_separator(char="=", length=60):
    """打印分隔线"""
    unreal.log(char * length)

def log_section(title):
    """打印章节标题"""
    log_separator()
    unreal.log(f"  {title}")
    log_separator()

def get_game_instance():
    """获取PIE模式下的GameInstance"""
    try:
        # 在PIE模式下，需要使用UnrealEditorSubsystem获取游戏世界
        editor_subsystem = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)
        game_world = editor_subsystem.get_game_world()
        
        if not game_world:
            unreal.log_error("❌ 无法获取游戏世界，请确保在PIE模式下运行")
            return None
        
        game_instance = game_world.get_game_instance()
        if not game_instance:
            unreal.log_error("❌ 无法获取GameInstance")
            return None
        
        return game_instance
    except Exception as e:
        unreal.log_error(f"❌ 获取GameInstance时发生错误: {str(e)}")
        return None

def test_initialization():
    """测试1: 初始化世界"""
    log_section("测试1: 初始化世界")
    
    try:
        # 获取GameInstance
        game_instance = get_game_instance()
        if not game_instance:
            return False
        
        # 创建默认参数
        params = unreal.SimulationParams()
        
        # 初始化世界
        unreal.log("正在初始化 50x50 网格...")
        unreal.WorldMorphingBlueprintLibrary.initialize_world(
            game_instance, 50, 50, params
        )
        
        # 验证网格尺寸
        width, height = unreal.WorldMorphingBlueprintLibrary.get_grid_size(game_instance)
        unreal.log(f"✅ 世界已初始化: {width}x{height}")
        
        # 验证时间步
        time_step = unreal.WorldMorphingBlueprintLibrary.get_time_step(game_instance)
        unreal.log(f"✅ 当前时间步: {time_step}")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 初始化失败: {str(e)}")
        return False

def test_simulation_update():
    """测试2: 模拟更新"""
    log_section("测试2: 模拟更新")
    
    try:
        game_instance = get_game_instance()
        if not game_instance:
            return False
        
        # 执行10次更新
        unreal.log("正在执行10次模拟更新...")
        for i in range(10):
            unreal.WorldMorphingBlueprintLibrary.tick_simulation(game_instance, 0.016)
        
        time_step = unreal.WorldMorphingBlueprintLibrary.get_time_step(game_instance)
        unreal.log(f"✅ 更新完成，当前时间步: {time_step}")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 模拟更新失败: {str(e)}")
        return False

def test_cell_reading():
    """测试3: 读取单元格状态"""
    log_section("测试3: 读取单元格状态")
    
    try:
        game_instance = get_game_instance()
        if not game_instance:
            return False
        
        # 读取中心区域的单元格
        unreal.log("正在读取中心区域单元格状态...")
        
        center_x, center_y = 25, 25
        cell_state = unreal.WorldMorphingBlueprintLibrary.get_cell_at(
            game_instance, center_x, center_y
        )
        
        unreal.log(f"位置 ({center_x}, {center_y}) 的单元格状态:")
        unreal.log(f"  - 存在地形: {cell_state.b_exists}")
        unreal.log(f"  - 地幔能量: {cell_state.mantle_energy:.2f}")
        unreal.log(f"  - 温度: {cell_state.temperature:.2f}")
        unreal.log(f"  - 晶石类型: {cell_state.crystal_type}")
        unreal.log(f"  - 储存能量: {cell_state.stored_energy:.2f}")
        
        # 统计不同类型的单元格数量
        unreal.log("\n正在统计单元格类型分布...")
        
        width, height = unreal.WorldMorphingBlueprintLibrary.get_grid_size(game_instance)
        
        stats = {
            'exists': 0,
            'empty': 0,
            'alpha': 0,
            'beta': 0,
            'human': 0,
            'thunderstorm': 0
        }
        
        for y in range(height):
            for x in range(width):
                cell = unreal.WorldMorphingBlueprintLibrary.get_cell_at(game_instance, x, y)
                if cell.b_exists:
                    stats['exists'] += 1
                    if cell.crystal_type == unreal.CrystalType.ALPHA:
                        stats['alpha'] += 1
                    elif cell.crystal_type == unreal.CrystalType.BETA:
                        stats['beta'] += 1
                    elif cell.crystal_type == unreal.CrystalType.HUMAN:
                        stats['human'] += 1
                    else:
                        stats['empty'] += 1
                    
                    if cell.b_has_thunderstorm:
                        stats['thunderstorm'] += 1
        
        unreal.log(f"✅ 统计完成:")
        unreal.log(f"  - 存在地形: {stats['exists']}/{width*height}")
        unreal.log(f"  - Alpha晶石: {stats['alpha']}")
        unreal.log(f"  - Beta晶石: {stats['beta']}")
        unreal.log(f"  - 人类聚落: {stats['human']}")
        unreal.log(f"  - 空地: {stats['empty']}")
        unreal.log(f"  - 雷暴区域: {stats['thunderstorm']}")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 读取单元格失败: {str(e)}")
        return False

def test_parameter_adjustment():
    """测试4: 参数调整"""
    log_section("测试4: 参数调整")
    
    try:
        game_instance = get_game_instance()
        if not game_instance:
            return False
        
        # 获取当前参数
        params = unreal.WorldMorphingBlueprintLibrary.get_simulation_params(game_instance)
        unreal.log(f"当前参数:")
        unreal.log(f"  - 扩张阈值: {params.expansion_threshold}")
        unreal.log(f"  - 雷暴阈值: {params.thunderstorm_threshold}")
        unreal.log(f"  - Alpha能量需求: {params.alpha_energy_demand}")
        
        # 修改参数
        unreal.log("\n正在修改参数...")
        params.expansion_threshold = 150.0
        params.thunderstorm_threshold = 15.0
        params.alpha_energy_demand = 1.0
        
        unreal.WorldMorphingBlueprintLibrary.set_simulation_params(game_instance, params)
        
        # 验证参数
        new_params = unreal.WorldMorphingBlueprintLibrary.get_simulation_params(game_instance)
        unreal.log(f"✅ 参数已更新:")
        unreal.log(f"  - 扩张阈值: {new_params.expansion_threshold}")
        unreal.log(f"  - 雷暴阈值: {new_params.thunderstorm_threshold}")
        unreal.log(f"  - Alpha能量需求: {new_params.alpha_energy_demand}")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 参数调整失败: {str(e)}")
        return False

def test_performance():
    """测试5: 性能基准测试"""
    log_section("测试5: 性能基准测试")
    
    try:
        game_instance = get_game_instance()
        if not game_instance:
            return False
        
        # 测试不同网格尺寸的性能
        test_sizes = [
            (30, 30),
            (50, 50),
            (80, 80),
        ]
        
        for width, height in test_sizes:
            unreal.log(f"\n测试 {width}x{height} 网格...")
            
            # 初始化
            params = unreal.WorldMorphingBlueprintLibrary.make_default_params()
            unreal.WorldMorphingBlueprintLibrary.initialize_world(
                game_instance, width, height, params
            )
            
            # 预热
            for _ in range(5):
                unreal.WorldMorphingBlueprintLibrary.tick_simulation(game_instance, 0.016)
            
            # 性能测试
            iterations = 100
            start_time = time.time()
            
            for _ in range(iterations):
                unreal.WorldMorphingBlueprintLibrary.tick_simulation(game_instance, 0.016)
            
            elapsed = time.time() - start_time
            avg_time = (elapsed / iterations) * 1000  # 转换为毫秒
            
            unreal.log(f"  ✅ 平均更新时间: {avg_time:.2f}ms/帧")
            unreal.log(f"  ✅ 理论帧率: {1000/avg_time:.1f} FPS")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 性能测试失败: {str(e)}")
        return False

def run_all_tests():
    """运行所有测试"""
    log_separator("=", 80)
    unreal.log("  WorldMorphingSystem 测试套件")
    log_separator("=", 80)
    
    unreal.log("\n⚠️  注意: 此测试需要在PIE (Play In Editor) 模式下运行")
    unreal.log("⚠️  请先点击 Play 按钮，然后在控制台运行此脚本\n")
    
    tests = [
        ("初始化世界", test_initialization),
        ("模拟更新", test_simulation_update),
        ("读取单元格", test_cell_reading),
        ("参数调整", test_parameter_adjustment),
        ("性能基准", test_performance),
    ]
    
    results = []
    
    for name, test_func in tests:
        try:
            result = test_func()
            results.append((name, result))
        except Exception as e:
            unreal.log_error(f"测试 '{name}' 发生异常: {str(e)}")
            results.append((name, False))
        
        unreal.log("")  # 空行分隔
    
    # 打印测试结果摘要
    log_section("测试结果摘要")
    
    passed = sum(1 for _, result in results if result)
    total = len(results)
    
    for name, result in results:
        status = "✅ 通过" if result else "❌ 失败"
        unreal.log(f"  {status} - {name}")
    
    unreal.log("")
    unreal.log(f"总计: {passed}/{total} 测试通过")
    
    if passed == total:
        unreal.log("🎉 所有测试通过！")
    else:
        unreal.log_warning(f"⚠️  有 {total - passed} 个测试失败")
    
    log_separator("=", 80)

def main():
    """主函数"""
    run_all_tests()

if __name__ == "__main__":
    main()
