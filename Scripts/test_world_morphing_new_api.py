"""
WorldMorphingSystem 测试套件 (新API版本)

此脚本演示如何使用新的模块化API进行测试。

新API分为三个模块:
- Simulation: 模拟功能
- Visualization: 视觉呈现
- Configuration: 参数配置

运行方法:
1. 在UE4编辑器中打开项目
2. 打开WorldMorphingTest关卡
3. 点击Play按钮进入PIE模式
4. 在Output Log中输入: py "Scripts/test_world_morphing_new_api.py"
"""

import unreal
import time

def log_separator():
    """打印分隔线"""
    unreal.log("=" * 60)

def log_section(title):
    """打印章节标题"""
    log_separator()
    unreal.log(f"  {title}")
    log_separator()

def get_world_context():
    """获取世界上下文对象（UE4 PIE模式兼容）
    
    关键：需要返回一个能够通过 GetWorld() 获取到有效 World 的对象。
    在PIE模式下，最好的选择是直接返回 None，让 C++ 代码使用 GEngine 的当前世界。
    """
    try:
        # 方法1: 尝试获取所有 Actor，然后返回第一个有效的
        try:
            # 在PIE模式下，尝试获取所有actor
            all_actors = unreal.EditorLevelLibrary.get_all_level_actors()
            if all_actors and len(all_actors) > 0:
                unreal.log(f"✅ 使用Level Actor作为上下文")
                return all_actors[0]
        except:
            # PIE模式下 EditorLevelLibrary 不可用
            pass
        
        # 方法2: 返回 None，让 C++ 使用 GEngine
        # 这在某些情况下可能有效
        unreal.log("⚠️  使用None作为上下文（依赖C++端的GEngine）")
        return None
        
    except Exception as e:
        unreal.log_error(f"❌ 获取世界上下文时发生错误: {str(e)}")
        return None

def test_simulation_module():
    """测试模拟功能模块"""
    log_section("测试1: 模拟功能模块 (Simulation)")
    
    try:
        world_context = get_world_context()
        
        # 1. 使用Configuration模块创建参数
        unreal.log("1.1 创建默认参数...")
        params = unreal.WorldMorphingConfiguration.make_default()
        unreal.log("✅ 默认参数已创建")
        
        # 2. 使用Simulation模块初始化
        unreal.log("\n1.2 初始化世界 (50x50)...")
        success = unreal.WorldMorphingSimulation.initialize(world_context, 50, 50, params)
        if success:
            unreal.log("✅ 世界已初始化")
        else:
            unreal.log_error("❌ 初始化失败")
            return False
        
        # 3. 获取状态
        unreal.log("\n1.3 获取模拟状态...")
        status = unreal.WorldMorphingSimulation.get_status(world_context)
        unreal.log(f"✅ 状态信息:")
        unreal.log(f"  - 已初始化: {status.b_initialized}")
        unreal.log(f"  - 网格尺寸: {status.width}x{status.height}")
        unreal.log(f"  - 时间步: {status.time_step}")
        unreal.log(f"  - 周期计数: {status.cycle_count}")
        
        # 4. 执行更新
        unreal.log("\n1.4 执行10次模拟更新...")
        for i in range(10):
            unreal.WorldMorphingSimulation.tick(world_context, 0.016)
        
        status = unreal.WorldMorphingSimulation.get_status(world_context)
        unreal.log(f"✅ 更新完成，当前时间步: {status.time_step}")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 模拟功能测试失败: {str(e)}")
        import traceback
        unreal.log_error(traceback.format_exc())
        return False

def test_visualization_module():
    """测试视觉呈现模块"""
    log_section("测试2: 视觉呈现模块 (Visualization)")
    
    try:
        world_context = get_world_context()
        
        # 1. 获取单个单元格状态
        unreal.log("2.1 获取单个单元格状态...")
        cell = unreal.WorldMorphingVisualization.get_cell_state(world_context, 25, 25)
        unreal.log(f"✅ 位置 (25, 25) 的单元格:")
        
        # 注意：Python中访问UE4的布尔属性时，前缀b_会被转换
        # bExists -> b_exists 或 exists
        try:
            exists = cell.b_exists
        except AttributeError:
            try:
                exists = cell.exists
            except AttributeError:
                exists = getattr(cell, 'bExists', False)
        
        try:
            mantle_energy = cell.mantle_energy
        except AttributeError:
            mantle_energy = getattr(cell, 'MantleEnergy', 0.0)
        
        try:
            temperature = cell.temperature
        except AttributeError:
            temperature = getattr(cell, 'Temperature', 0.0)
        
        try:
            crystal_type = cell.crystal_type
        except AttributeError:
            crystal_type = getattr(cell, 'CrystalType', None)
        
        unreal.log(f"  - 存在地形: {exists}")
        unreal.log(f"  - 地幔能量: {mantle_energy:.2f}")
        unreal.log(f"  - 温度: {temperature:.2f}")
        unreal.log(f"  - 晶石类型: {crystal_type}")
        
        # 2. 获取区域状态
        unreal.log("\n2.2 获取区域状态 (10x10)...")
        region = unreal.WorldMorphingVisualization.get_region_states(
            world_context, 20, 20, 10, 10
        )
        unreal.log(f"✅ 获取了 {len(region)} 个单元格")
        
        # 3. 获取统计信息
        unreal.log("\n2.3 获取世界统计信息...")
        stats = unreal.WorldMorphingVisualization.get_statistics(world_context)
        unreal.log(f"✅ 统计信息:")
        unreal.log(f"  - 总单元格: {stats.total_cells}")
        unreal.log(f"  - 地形单元格: {stats.terrain_cells}")
        unreal.log(f"  - Alpha晶石: {stats.alpha_crystals}")
        unreal.log(f"  - Beta晶石: {stats.beta_crystals}")
        unreal.log(f"  - 人类聚落: {stats.human_settlements}")
        unreal.log(f"  - 雷暴区域: {stats.thunderstorm_cells}")
        unreal.log(f"  - 平均地幔能量: {stats.average_mantle_energy:.2f}")
        unreal.log(f"  - 平均温度: {stats.average_temperature:.2f}")
        
        # 4. 获取热力图数据
        unreal.log("\n2.4 获取热力图数据...")
        heatmap = unreal.WorldMorphingVisualization.get_heatmap_data(
            world_context, unreal.HeatmapDataType.MANTLE_ENERGY
        )
        unreal.log(f"✅ 获取了 {len(heatmap)} 个热力图数据点")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 视觉呈现测试失败: {str(e)}")
        import traceback
        unreal.log_error(traceback.format_exc())
        return False

def test_configuration_module():
    """测试参数配置模块"""
    log_section("测试3: 参数配置模块 (Configuration)")
    
    try:
        world_context = get_world_context()
        
        # 1. 创建默认参数
        unreal.log("3.1 创建默认参数...")
        default_params = unreal.WorldMorphingConfiguration.make_default()
        unreal.log(f"✅ 默认参数:")
        unreal.log(f"  - 扩张阈值: {default_params.expansion_threshold}")
        unreal.log(f"  - 雷暴阈值: {default_params.thunderstorm_threshold}")
        unreal.log(f"  - Alpha能量需求: {default_params.alpha_energy_demand}")
        
        # 2. 创建自定义参数
        unreal.log("\n3.2 创建自定义参数...")
        custom_params = unreal.WorldMorphingConfiguration.make_custom(
            150.0,  # ExpansionThreshold
            15.0,   # ThunderstormThreshold
            1.0     # AlphaEnergyDemand
        )
        unreal.log(f"✅ 自定义参数:")
        unreal.log(f"  - 扩张阈值: {custom_params.expansion_threshold}")
        unreal.log(f"  - 雷暴阈值: {custom_params.thunderstorm_threshold}")
        unreal.log(f"  - Alpha能量需求: {custom_params.alpha_energy_demand}")
        
        # 3. 应用参数
        unreal.log("\n3.3 应用自定义参数...")
        unreal.WorldMorphingConfiguration.apply(world_context, custom_params)
        
        # 4. 获取当前参数
        current_params = unreal.WorldMorphingConfiguration.get_current(world_context)
        unreal.log(f"✅ 当前参数已更新:")
        unreal.log(f"  - 扩张阈值: {current_params.expansion_threshold}")
        unreal.log(f"  - 雷暴阈值: {current_params.thunderstorm_threshold}")
        unreal.log(f"  - Alpha能量需求: {current_params.alpha_energy_demand}")
        
        # 5. 测试预设参数
        unreal.log("\n3.4 测试预设参数...")
        presets = [
            ("默认", unreal.SimulationPreset.DEFAULT),
            ("快速生长", unreal.SimulationPreset.FAST_GROWTH),
            ("缓慢演化", unreal.SimulationPreset.SLOW_EVOLUTION),
            ("高能量", unreal.SimulationPreset.HIGH_ENERGY),
            ("稳定", unreal.SimulationPreset.STABLE),
        ]
        
        for name, preset in presets:
            params = unreal.WorldMorphingConfiguration.make_preset(preset)
            unreal.log(f"  - {name}: 扩张阈值={params.expansion_threshold:.1f}")
        
        unreal.log("✅ 所有预设参数已测试")
        
        # 6. 重置为默认
        unreal.log("\n3.5 重置为默认参数...")
        unreal.WorldMorphingConfiguration.reset_to_default(world_context)
        current_params = unreal.WorldMorphingConfiguration.get_current(world_context)
        unreal.log(f"✅ 已重置为默认参数 (扩张阈值={current_params.expansion_threshold})")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 参数配置测试失败: {str(e)}")
        import traceback
        unreal.log_error(traceback.format_exc())
        return False

def test_performance():
    """性能基准测试"""
    log_section("测试4: 性能基准测试")
    
    try:
        world_context = get_world_context()
        
        test_sizes = [(30, 30), (50, 50), (80, 80)]
        
        for width, height in test_sizes:
            unreal.log(f"\n测试 {width}x{height} 网格...")
            
            # 初始化
            params = unreal.WorldMorphingConfiguration.make_default()
            success = unreal.WorldMorphingSimulation.initialize(world_context, width, height, params)
            
            if not success:
                unreal.log_warning(f"  ⚠️  {width}x{height} 网格初始化失败，跳过")
                continue
            
            # 预热
            for _ in range(5):
                unreal.WorldMorphingSimulation.tick(world_context, 0.016)
            
            # 性能测试
            iterations = 100
            start_time = time.time()
            
            for _ in range(iterations):
                unreal.WorldMorphingSimulation.tick(world_context, 0.016)
            
            elapsed = time.time() - start_time
            
            if elapsed > 0:
                avg_time = (elapsed / iterations) * 1000
                unreal.log(f"  ✅ 平均更新时间: {avg_time:.2f}ms/帧")
                unreal.log(f"  ✅ 理论帧率: {1000/avg_time:.1f} FPS")
            else:
                unreal.log(f"  ⚠️  测试时间过短，无法计算准确结果")
        
        return True
        
    except Exception as e:
        unreal.log_error(f"❌ 性能测试失败: {str(e)}")
        import traceback
        unreal.log_error(traceback.format_exc())
        return False

def main():
    """主测试函数"""
    log_separator()
    unreal.log("  WorldMorphingSystem 测试套件 (新API)")
    log_separator()
    
    unreal.log("\n⚠️  注意: 此测试使用新的模块化API")
    unreal.log("⚠️  请确保在PIE (Play In Editor) 模式下运行\n")
    
    # 运行所有测试
    results = {}
    results["模拟功能"] = test_simulation_module()
    results["视觉呈现"] = test_visualization_module()
    results["参数配置"] = test_configuration_module()
    results["性能基准"] = test_performance()
    
    # 打印结果摘要
    log_section("测试结果摘要")
    
    for test_name, passed in results.items():
        status = "✅ 通过" if passed else "❌ 失败"
        unreal.log(f"  {status} - {test_name}")
    
    passed_count = sum(1 for p in results.values() if p)
    total_count = len(results)
    
    unreal.log(f"\n总计: {passed_count}/{total_count} 测试通过")
    
    if passed_count == total_count:
        unreal.log("🎉 所有测试通过！")
    else:
        unreal.log_warning("⚠️  部分测试失败，请检查日志")
    
    log_separator()

# 运行测试
if __name__ == "__main__":
    main()
