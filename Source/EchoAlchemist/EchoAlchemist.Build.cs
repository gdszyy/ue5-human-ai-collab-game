// Copyright 2025 Voidzyy. All Rights Reserved.

using UnrealBuildTool;

public class EchoAlchemist : ModuleRules
{
	public EchoAlchemist(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"Niagara",  // 混合物理系统需要Niagara粒子系统
			"Paper2D"   // MonsterVisualizer需要Paper2D插件
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// 添加自动化测试依赖
		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEd"
			});
		}

		// 添加自动化测试框架支持（包括 WITH_AUTOMATION_TESTS 宏）
		if (Target.bBuildDeveloperTools || (Target.Configuration != UnrealTargetConfiguration.Shipping && Target.Configuration != UnrealTargetConfiguration.Test))
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"AutomationController"
			});
		}

		// 如果需要使用 Remote Control API
		// PrivateDependencyModuleNames.AddRange(new string[] { "RemoteControl" });
	}
}
