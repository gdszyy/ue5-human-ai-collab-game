// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageInfo.h"
#include "MarbleState.h"
#include "EnemyData.h"
#include "DamageCalculator.generated.h"

/**
 * 伤害计算器
 * 负责所有伤害计算，包括弹射药剂的伤害递增、药效强度消耗等
 * 
 * 设计理念：
 * - 纯计算：不依赖任何外部状态，所有计算都是纯函数
 * - 可测试性：所有方法都是静态的，易于单元测试
 * - 数据驱动：所有计算参数都来自数据结构
 * 
 * 核心功能：
 * 1. 计算弹射药剂的伤害递增（基于自然数和序列 Sn = 1+2+...+n）
 * 2. 计算药效强度削减（药效不足时按比例削减）
 * 3. 计算最终伤害（基础伤害 + 伤害加成 × 药效削减系数）
 */
UCLASS()
class ECHOALCHEMIST_API UDamageCalculator : public UObject
{
	GENERATED_BODY()

public:
	// ========== 核心计算方法 ==========
	
	/**
	 * 计算伤害
	 * @param Marble 魔力露珠/魔药状态
	 * @param TargetID 目标敌人ID
	 * @return 伤害信息
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Damage")
	static FDamageInfo CalculateDamage(const FMarbleState& Marble, FGuid TargetID);

	/**
	 * 计算弹射药剂的伤害加成
	 * 基于自然数和序列 Sn = 1+2+...+n
	 * 
	 * 伤害加成规则：
	 * - 撞击次数达到 S1=1 时，伤害加成从 0 增加到 +1
	 * - 撞击次数达到 S2=3 时，伤害加成从 +1 增加到 +2
	 * - 撞击次数达到 S3=6 时，伤害加成从 +2 增加到 +3
	 * - 以此类推...
	 * 
	 * @param HitCount 撞击次数
	 * @return 伤害加成
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Damage")
	static float CalculateRicochetDamageBonus(int32 HitCount);

	/**
	 * 计算药效强度削减系数
	 * 
	 * 削减规则：
	 * - 如果剩余药效 >= 需求药效，削减系数为 1.0（无削减）
	 * - 如果剩余药效 < 需求药效，削减系数为 剩余药效 / 需求药效
	 * - 如果剩余药效 <= 0，削减系数为 0.0（无伤害）
	 * 
	 * @param PotencyRemaining 剩余药效强度
	 * @param PotencyRequired 需求药效强度
	 * @return 削减系数（0.0 - 1.0）
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Damage")
	static float CalculatePotencyReduction(float PotencyRemaining, float PotencyRequired);

	/**
	 * 获取魔药类型的药效强度需求
	 * 
	 * 需求规则：
	 * - 弹射药剂（Ricochet）：1.0
	 * - 穿刺精粹（Piercing）：1.5
	 * - 爆裂灵液（Explosive）：3.0
	 * 
	 * @param PotionType 魔药类型
	 * @return 药效强度需求
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Damage")
	static float GetPotencyRequired(EPotionType PotionType);

	// ========== 辅助计算方法 ==========
	
	/**
	 * 计算暴击伤害
	 * @param BaseDamage 基础伤害
	 * @param CriticalChance 暴击几率（0.0 - 1.0）
	 * @param CriticalMultiplier 暴击倍率
	 * @param OutIsCritical 是否暴击
	 * @return 最终伤害
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Damage")
	static float CalculateCriticalDamage(
		float BaseDamage,
		float CriticalChance,
		float CriticalMultiplier,
		bool& OutIsCritical
	);

	/**
	 * 计算护甲减伤
	 * @param Damage 原始伤害
	 * @param Armor 护甲值
	 * @return 减伤后的伤害
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Damage")
	static float CalculateArmorReduction(float Damage, float Armor);

	// ========== 调试方法 ==========
	
	/**
	 * 打印伤害信息（用于调试）
	 * @param DamageInfo 伤害信息
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Damage|Debug")
	static void PrintDamageInfo(const FDamageInfo& DamageInfo);

	/**
	 * 生成伤害加成表（用于调试和文档）
	 * @param MaxHitCount 最大撞击次数
	 * @return 伤害加成表
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Damage|Debug")
	static TArray<FString> GenerateDamageBonusTable(int32 MaxHitCount = 20);

private:
	// ========== 内部辅助方法 ==========
	
	/**
	 * 计算自然数和序列的第k项
	 * Sk = 1 + 2 + ... + k = k * (k + 1) / 2
	 * @param k 项数
	 * @return 第k项的值
	 */
	static int32 CalculateNaturalSum(int32 k);
};
