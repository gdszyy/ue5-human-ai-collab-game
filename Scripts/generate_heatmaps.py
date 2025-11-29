"""
生成WorldMorphingSystem热力图

此脚本在外部Python环境中运行，读取导出的数据并生成可视化图片。

运行方法：
1. 确保已安装依赖: pip install matplotlib numpy
2. 先在UE4中运行 visualize_world_morphing.py 导出数据
3. 在外部Python环境运行: python Scripts/generate_heatmaps.py

输出：
- Saved/world_morphing_mantle.png
- Saved/world_morphing_temperature.png
- Saved/world_morphing_crystals.png
- Saved/world_morphing_combined.png
"""

import json
import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

def load_data():
    """加载导出的数据"""
    # 查找项目根目录
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_dir = os.path.dirname(script_dir)
    data_file = os.path.join(project_dir, "Saved", "world_morphing_data.json")
    
    if not os.path.exists(data_file):
        print(f"❌ 数据文件不存在: {data_file}")
        print("请先在UE4编辑器中运行 visualize_world_morphing.py")
        return None
    
    with open(data_file, 'r') as f:
        data = json.load(f)
    
    print(f"✅ 数据加载成功: {data['width']}x{data['height']}")
    return data

def plot_mantle_energy(data, output_dir):
    """绘制地幔能量热力图"""
    mantle = np.array(data['mantle_energy'])
    exists = np.array(data['exists'])
    
    # 只显示存在地形的区域
    mantle_masked = np.where(exists == 1, mantle, np.nan)
    
    plt.figure(figsize=(10, 10))
    plt.imshow(mantle_masked, cmap='hot', interpolation='nearest', origin='lower')
    plt.colorbar(label='Mantle Energy')
    plt.title('Mantle Energy Distribution')
    plt.xlabel('X')
    plt.ylabel('Y')
    
    output_path = os.path.join(output_dir, 'world_morphing_mantle.png')
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()
    
    print(f"✅ 地幔能量图已保存: {output_path}")

def plot_temperature(data, output_dir):
    """绘制温度热力图"""
    temperature = np.array(data['temperature'])
    exists = np.array(data['exists'])
    thunderstorm = np.array(data['thunderstorm'])
    
    # 只显示存在地形的区域
    temp_masked = np.where(exists == 1, temperature, np.nan)
    
    plt.figure(figsize=(10, 10))
    im = plt.imshow(temp_masked, cmap='coolwarm', interpolation='nearest', origin='lower', vmin=-50, vmax=50)
    plt.colorbar(im, label='Temperature (°C)')
    
    # 标记雷暴区域
    thunder_y, thunder_x = np.where(thunderstorm == 1)
    plt.scatter(thunder_x, thunder_y, c='yellow', s=10, marker='*', alpha=0.6, label='Thunderstorm')
    
    plt.title('Temperature Distribution')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.legend()
    
    output_path = os.path.join(output_dir, 'world_morphing_temperature.png')
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()
    
    print(f"✅ 温度图已保存: {output_path}")

def plot_crystals(data, output_dir):
    """绘制晶石分布图"""
    crystals = np.array(data['crystal_type'])
    
    # 定义颜色映射
    # -1: 不存在 (黑色)
    # 0: Empty (灰色)
    # 1: Alpha (绿色)
    # 2: Beta (红色)
    # 3: Human (蓝色)
    colors = ['black', 'lightgray', 'lime', 'red', 'blue']
    cmap = ListedColormap(colors)
    
    plt.figure(figsize=(10, 10))
    im = plt.imshow(crystals, cmap=cmap, interpolation='nearest', origin='lower', vmin=-1, vmax=3)
    
    # 添加颜色条
    cbar = plt.colorbar(im, ticks=[-1, 0, 1, 2, 3])
    cbar.set_label('Crystal Type')
    cbar.ax.set_yticklabels(['Non-exist', 'Empty', 'Alpha', 'Beta', 'Human'])
    
    plt.title('Crystal Distribution')
    plt.xlabel('X')
    plt.ylabel('Y')
    
    output_path = os.path.join(output_dir, 'world_morphing_crystals.png')
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()
    
    print(f"✅ 晶石分布图已保存: {output_path}")

def plot_combined(data, output_dir):
    """绘制综合视图"""
    fig, axes = plt.subplots(2, 2, figsize=(16, 16))
    
    mantle = np.array(data['mantle_energy'])
    temperature = np.array(data['temperature'])
    crystals = np.array(data['crystal_type'])
    exists = np.array(data['exists'])
    
    # 地幔能量
    mantle_masked = np.where(exists == 1, mantle, np.nan)
    im1 = axes[0, 0].imshow(mantle_masked, cmap='hot', interpolation='nearest', origin='lower')
    axes[0, 0].set_title('Mantle Energy')
    axes[0, 0].set_xlabel('X')
    axes[0, 0].set_ylabel('Y')
    plt.colorbar(im1, ax=axes[0, 0])
    
    # 温度
    temp_masked = np.where(exists == 1, temperature, np.nan)
    im2 = axes[0, 1].imshow(temp_masked, cmap='coolwarm', interpolation='nearest', origin='lower', vmin=-50, vmax=50)
    axes[0, 1].set_title('Temperature')
    axes[0, 1].set_xlabel('X')
    axes[0, 1].set_ylabel('Y')
    plt.colorbar(im2, ax=axes[0, 1])
    
    # 晶石分布
    colors = ['black', 'lightgray', 'lime', 'red', 'blue']
    cmap = ListedColormap(colors)
    im3 = axes[1, 0].imshow(crystals, cmap=cmap, interpolation='nearest', origin='lower', vmin=-1, vmax=3)
    axes[1, 0].set_title('Crystal Distribution')
    axes[1, 0].set_xlabel('X')
    axes[1, 0].set_ylabel('Y')
    cbar3 = plt.colorbar(im3, ax=axes[1, 0], ticks=[-1, 0, 1, 2, 3])
    cbar3.ax.set_yticklabels(['Non-exist', 'Empty', 'Alpha', 'Beta', 'Human'])
    
    # 地形存在性
    im4 = axes[1, 1].imshow(exists, cmap='gray', interpolation='nearest', origin='lower')
    axes[1, 1].set_title('Terrain Existence')
    axes[1, 1].set_xlabel('X')
    axes[1, 1].set_ylabel('Y')
    plt.colorbar(im4, ax=axes[1, 1])
    
    plt.suptitle(f'WorldMorphingSystem - Combined View ({data["width"]}x{data["height"]})', fontsize=16)
    plt.tight_layout()
    
    output_path = os.path.join(output_dir, 'world_morphing_combined.png')
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()
    
    print(f"✅ 综合视图已保存: {output_path}")

def main():
    """主函数"""
    print("=" * 60)
    print("  WorldMorphingSystem 热力图生成器")
    print("=" * 60)
    print()
    
    # 加载数据
    data = load_data()
    if data is None:
        return
    
    # 确定输出目录
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_dir = os.path.dirname(script_dir)
    output_dir = os.path.join(project_dir, "Saved")
    
    # 生成图片
    print("\n正在生成热力图...")
    plot_mantle_energy(data, output_dir)
    plot_temperature(data, output_dir)
    plot_crystals(data, output_dir)
    plot_combined(data, output_dir)
    
    print("\n" + "=" * 60)
    print("✅ 所有热力图生成完成！")
    print(f"输出目录: {output_dir}")
    print("=" * 60)

if __name__ == "__main__":
    main()
