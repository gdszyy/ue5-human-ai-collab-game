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
			"InputCore" 
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// 添加自动化测试依赖
		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.AddRange(new string[] {
				"UnrealEd",
				"AutomationController"
			});
		}

		// 如果需要使用 Remote Control API
		// PrivateDependencyModuleNames.AddRange(new string[] { "RemoteControl" });
	}
}
