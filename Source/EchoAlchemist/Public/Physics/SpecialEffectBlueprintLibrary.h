// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Physics/SpecialEffectData.h"
#include "SpecialEffectBlueprintLibrary.generated.h"

/**
 * 特殊效果蓝图函数库
 * 
 * 提供便捷的蓝图接口用于创建和配置特殊效果参数。
 * 
 * 蓝图使用示例：
 * 
 * 1. 创建引力奇点参数
 *    ```
 *    FGravityWellParams Params = USpecialEffectBlueprintLibrary::MakeGravityWellParams(
 *        Position,
 *        1000.0f,  // GravityStrength
 *        500.0f,   // EffectRadius
 *        5.0f      // Duration
 *    );
 *    ```
 * 
 * 2. 创建分裂参数
 *    ```
 *    FSplitParams Params = USpecialEffectBlueprintLibrary::MakeSplitParams(
 *        3,        // SplitCount
 *        30.0f,    // SplitAngle
 *        0.8f,     // SpeedMultiplier
 *        0.5f      // PotencyMultiplier
 *    );
 *    ```
 */
UCLASS()
class ECHOALCHEMIST_API USpecialEffectBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ========== 参数构造函数 ==========
	
	/**
	 * 创建引力奇点参数
	 * 
	 * @param Position 奇点位置
	 * @param GravityStrength 引力强度（单位：cm/s²）
	 * @param EffectRadius 影响半径（单位：cm）
	 * @param Duration 持续时间（单位：秒，0表示永久）
	 * @param bDestroyOnReach 是否在弹珠到达中心时销毁
	 * @return 引力奇点参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Params", meta = (Keywords = "create make gravity well"))
	static FGravityWellParams MakeGravityWellParams(
		FVector Position,
		float GravityStrength = 1000.0f,
		float EffectRadius = 500.0f,
		float Duration = 5.0f,
		bool bDestroyOnReach = false
	);

	/**
	 * 创建虫洞传送参数
	 * 
	 * @param EntrancePosition 虫洞入口位置
	 * @param ExitPosition 虫洞出口位置
	 * @param EntranceRadius 入口半径（单位：cm）
	 * @param ExitSpeedMultiplier 出口速度倍率
	 * @param Duration 持续时间（单位：秒，0表示永久）
	 * @return 虫洞传送参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Params", meta = (Keywords = "create make wormhole portal"))
	static FWormholeParams MakeWormholeParams(
		FVector EntrancePosition,
		FVector ExitPosition,
		float EntranceRadius = 50.0f,
		float ExitSpeedMultiplier = 1.0f,
		float Duration = 10.0f
	);

	/**
	 * 创建弹珠分裂参数
	 * 
	 * @param SplitCount 分裂数量
	 * @param SplitAngle 分裂角度（单位：度）
	 * @param SpeedMultiplier 子弹珠速度倍率
	 * @param PotencyMultiplier 子弹珠药效倍率
	 * @param RadiusMultiplier 子弹珠半径倍率
	 * @return 弹珠分裂参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Params", meta = (Keywords = "create make split divide"))
	static FSplitParams MakeSplitParams(
		int32 SplitCount = 2,
		float SplitAngle = 30.0f,
		float SpeedMultiplier = 0.8f,
		float PotencyMultiplier = 0.5f,
		float RadiusMultiplier = 0.7f
	);

	/**
	 * 创建速度修改参数
	 * 
	 * @param SpeedMultiplier 速度倍率（>1加速，<1减速）
	 * @param bApplyImmediately 是否立即应用
	 * @param Duration 持续时间（单位：秒，0表示瞬间）
	 * @return 速度修改参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Params", meta = (Keywords = "create make speed modifier boost slow"))
	static FEchoSpeedModifierParams MakeSpeedModifierParams(
		float SpeedMultiplier = 1.5f,
		bool bApplyImmediately = true,
		float Duration = 0.0f
	);

	/**
	 * 创建连锁触发参数
	 * 
	 * @param SecondaryCount 次级弹珠数量
	 * @param AngleRange 发射角度范围（单位：度）
	 * @param SecondarySpeed 次级弹珠速度
	 * @param PotencyMultiplier 次级弹珠药效倍率
	 * @param SecondaryRadius 次级弹珠半径
	 * @return 连锁触发参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Params", meta = (Keywords = "create make chain trigger secondary"))
	static FChainTriggerParams MakeChainTriggerParams(
		int32 SecondaryCount = 3,
		float AngleRange = 60.0f,
		float SecondarySpeed = 800.0f,
		float PotencyMultiplier = 0.3f,
		float SecondaryRadius = 5.0f
	);

	// ========== 预设参数 ==========
	
	/**
	 * 创建弱引力奇点参数（用于吸引效果）
	 * 
	 * @param Position 奇点位置
	 * @return 引力奇点参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Presets", meta = (Keywords = "preset weak gravity well"))
	static FGravityWellParams MakeWeakGravityWell(FVector Position);

	/**
	 * 创建强引力奇点参数（用于黑洞效果）
	 * 
	 * @param Position 奇点位置
	 * @return 引力奇点参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Presets", meta = (Keywords = "preset strong gravity well black hole"))
	static FGravityWellParams MakeStrongGravityWell(FVector Position);

	/**
	 * 创建二分裂参数（分裂成2个）
	 * 
	 * @return 分裂参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Presets", meta = (Keywords = "preset binary split two"))
	static FSplitParams MakeBinarySplit();

	/**
	 * 创建三分裂参数（分裂成3个）
	 * 
	 * @return 分裂参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Presets", meta = (Keywords = "preset triple split three"))
	static FSplitParams MakeTripleSplit();

	/**
	 * 创建加速参数（2倍速）
	 * 
	 * @return 速度修改参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Presets", meta = (Keywords = "preset speed boost fast"))
	static FEchoSpeedModifierParams MakeSpeedBoost();

	/**
	 * 创建减速参数（0.5倍速）
	 * 
	 * @return 速度修改参数
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Presets", meta = (Keywords = "preset speed slow"))
	static FEchoSpeedModifierParams MakeSpeedSlow();

	// ========== 辅助函数 ==========
	
	/**
	 * 获取效果类型的显示名称
	 * 
	 * @param EffectType 效果类型
	 * @return 显示名称
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Utility", meta = (Keywords = "get effect type name"))
	static FString GetEffectTypeName(EEchoSpecialEffectType EffectType);

	/**
	 * 检查效果类型是否为持续性效果
	 * 
	 * @param EffectType 效果类型
	 * @return true=持续性效果，false=瞬间效果
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect|Utility", meta = (Keywords = "check effect type continuous"))
	static bool IsContinuousEffect(EEchoSpecialEffectType EffectType);
};
