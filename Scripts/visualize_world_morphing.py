"""
WorldMorphingSystem 可视化测试脚本

此脚本在UE4编辑器内运行，生成世界状态的可视化数据并导出为图片。

运行方法：
1. 确保已安装 matplotlib: pip install matplotlib
2. 在 UE4 编辑器中运行 PIE 模式
3. 在 Output Log 中输入：py "Scripts/visualize_world_morphing.py"

输出：
- world_morphing_mantle.png - 地幔能量热力图
- world_morphing_temperature.png - 温度热力图
- world_morphing_crystals.png - 晶石分布图
- world_morphing_combined.png - 综合视图
"""

import unreal
import os

def get_world_context():
    """获取世界上下文对象（UE4 PIE模式兼容）"""
    try:
        # 方法1: 尝试通过 GameInstance 获取（PIE模式下最可靠）
        try:
            all_worlds = unreal.EditorLevelLibrary.get_all_level_actors()
        except:
            # PIE模式下 EditorLevelLibrary 不可用，这是预期的
            pass
        
        # 方法2: 使用 SystemLibrary 获取游戏实例（PIE模式推荐）
        try:
            game_instance_class = unreal.load_object(None, '/Script/Engine.GameInstance')
            if game_instance_class:
                default_obj = unreal.get_default_object(game_instance_class)
                if default_obj:
                    return default_obj
        except Exception as e:
            unreal.log(f"方法2失败: {str(e)}")
            pass
        
        # 方法3: 尝试使用 World 类的默认对象
        try:
            world_class = unreal.load_object(None, '/Script/Engine.World')
            if world_class:
                default_world = unreal.get_default_object(world_class)
                if default_world:
                    return default_world
        except Exception as e:
            unreal.log(f"方法3失败: {str(e)}")
            pass
        
        # 方法4: 创建一个新的对象作为上下文
        try:
            temp_obj = unreal.new_object(unreal.Object)
            if temp_obj:
                return temp_obj
        except Exception as e:
            unreal.log(f"方法4失败: {str(e)}")
            pass
        
        # 方法5: 使用 GameplayStatics 的默认对象
        try:
            statics_class = unreal.GameplayStatics
            if statics_class:
                return statics_class
        except Exception as e:
            unreal.log(f"方法5失败: {str(e)}")
            pass
        
        unreal.log_error("❌ 无法获取WorldContext")
        return None
        
    except Exception as e:
        unreal.log_error(f"❌ 错误: {str(e)}")
        return None

def export_world_state_data():
    """导出世界状态数据"""
    try:
        world_context = get_world_context()
        if not world_context:
            return None
        
        # 获取网格尺寸
        width, height = unreal.WorldMorphingBlueprintLibrary.get_grid_size(world_context)
        
        if width == 0 or height == 0:
            unreal.log_error("❌ 世界未初始化，请先运行测试脚本")
            return None
        
        unreal.log(f"正在导出 {width}x{height} 网格数据...")
        
        # 读取所有单元格数据
        data = {
            'width': width,
            'height': height,
            'mantle_energy': [],
            'temperature': [],
            'crystal_type': [],
            'exists': [],
            'thunderstorm': []
        }
        
        for y in range(height):
            mantle_row = []
            temp_row = []
            crystal_row = []
            exists_row = []
            thunder_row = []
            
            for x in range(width):
                cell = unreal.WorldMorphingBlueprintLibrary.get_cell_at(world_context, x, y)
                
                mantle_row.append(cell.mantle_energy if cell.b_exists else 0)
                temp_row.append(cell.temperature if cell.b_exists else -100)
                
                # 晶石类型转换为数字
                if not cell.b_exists:
                    crystal_row.append(-1)
                elif cell.crystal_type == unreal.CrystalType.EMPTY:
                    crystal_row.append(0)
                elif cell.crystal_type == unreal.CrystalType.ALPHA:
                    crystal_row.append(1)
                elif cell.crystal_type == unreal.CrystalType.BETA:
                    crystal_row.append(2)
                elif cell.crystal_type == unreal.CrystalType.HUMAN:
                    crystal_row.append(3)
                else:
                    crystal_row.append(0)
                
                exists_row.append(1 if cell.b_exists else 0)
                thunder_row.append(1 if cell.b_has_thunderstorm else 0)
            
            data['mantle_energy'].append(mantle_row)
            data['temperature'].append(temp_row)
            data['crystal_type'].append(crystal_row)
            data['exists'].append(exists_row)
            data['thunderstorm'].append(thunder_row)
        
        unreal.log("✅ 数据导出完成")
        return data
        
    except Exception as e:
        unreal.log_error(f"❌ 导出数据失败: {str(e)}")
        return None

def save_data_to_file(data, filename):
    """保存数据到文件"""
    import json
    
    project_dir = unreal.SystemLibrary.get_project_directory()
    output_path = os.path.join(project_dir, "Saved", filename)
    
    # 确保目录存在
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    with open(output_path, 'w') as f:
        json.dump(data, f)
    
    unreal.log(f"✅ 数据已保存到: {output_path}")
    return output_path

def generate_visualizations():
    """生成可视化图片（需要在外部Python环境运行）"""
    unreal.log("=" * 60)
    unreal.log("  WorldMorphingSystem 可视化")
    unreal.log("=" * 60)
    
    # 导出数据
    data = export_world_state_data()
    
    if data is None:
        return
    
    # 保存数据
    data_file = save_data_to_file(data, "world_morphing_data.json")
    
    unreal.log("\n" + "=" * 60)
    unreal.log("数据导出完成！")
    unreal.log("=" * 60)
    unreal.log("\n要生成可视化图片，请在外部Python环境中运行:")
    unreal.log(f"  python Scripts/generate_heatmaps.py")
    unreal.log("")

def main():
    """主函数"""
    generate_visualizations()

if __name__ == "__main__":
    main()
