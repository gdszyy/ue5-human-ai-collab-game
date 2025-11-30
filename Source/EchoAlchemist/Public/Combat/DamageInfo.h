// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MarbleState.h"
#include "DamageInfo.generated.h"

/**
 * 伤害信息
 * 描述一次伤害事件的所有信息
 * 
 * 设计理念：
 * - 完整记录：记录伤害计算的所有中间过程
 * - 可追溯性：便于调试和平衡性调整
 * - 事件驱动：作为伤害事件的数据载体
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FDamageInfo
{
	GENERATED_BODY()

	// ========== 伤害来源 ==========
	
	/** 伤害来源ID（魔力露珠/魔药的ID） */
	UPROPERTY(BlueprintReadWrite, Category = "Source")
	FGuid SourceID;

	/** 魔药类型 */
	UPROPERTY(BlueprintReadWrite, Category = "Source")
	EPotionType PotionType = EPotionType::Ricochet;

	// ========== 伤害目标 ==========
	
	/** 伤害目标ID（敌人的ID） */
	UPROPERTY(BlueprintReadWrite, Category = "Target")
	FGuid TargetID;

	// ========== 伤害数值 ==========
	
	/** 基础伤害 */
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	float BaseDamage = 0.0f;

	/** 伤害加成（弹射药剂的伤害递增） */
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	float DamageBonus = 0.0f;

	/** 最终伤害 */
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	float FinalDamage = 0.0f;

	// ========== 伤害计算参数 ==========
	
	/** 撞击次数（用于计算弹射药剂的伤害递增） */
	UPROPERTY(BlueprintReadWrite, Category = "Calculation")
	int32 HitCount = 0;

	/** 剩余药效强度 */
	UPROPERTY(BlueprintReadWrite, Category = "Calculation")
	float PotencyRemaining = 0.0f;

	/** 需求药效强度 */
	UPROPERTY(BlueprintReadWrite, Category = "Calculation")
	float PotencyRequired = 0.0f;

	/** 药效削减系数（0.0 - 1.0） */
	UPROPERTY(BlueprintReadWrite, Category = "Calculation")
	float PotencyReduction = 1.0f;

	// ========== 伤害结果 ==========
	
	/** 是否为致命伤害 */
	UPROPERTY(BlueprintReadWrite, Category = "Result")
	bool bIsLethal = false;

	/** 是否为暴击 */
	UPROPERTY(BlueprintReadWrite, Category = "Result")
	bool bIsCritical = false;

	/** 伤害时间戳 */
	UPROPERTY(BlueprintReadWrite, Category = "Result")
	float Timestamp = 0.0f;

	// ========== 辅助方法 ==========
	
	/**
	 * 获取伤害描述字符串（用于调试）
	 * @return 伤害描述字符串
	 */
	FString ToString() const
	{
		return FString::Printf(
			TEXT("Damage: %.1f (Base: %.1f + Bonus: %.1f) x Reduction: %.2f | HitCount: %d | Potency: %.1f/%.1f | Lethal: %s"),
			FinalDamage,
			BaseDamage,
			DamageBonus,
			PotencyReduction,
			HitCount,
			PotencyRemaining,
			PotencyRequired,
			bIsLethal ? TEXT("Yes") : TEXT("No")
		);
	}

	/**
	 * 获取伤害类型名称
	 * @return 伤害类型名称
	 */
	FString GetPotionTypeName() const
	{
		switch (PotionType)
		{
		case EPotionType::Ricochet:
			return TEXT("Ricochet");
		case EPotionType::Piercing:
			return TEXT("Piercing");
		case EPotionType::Explosive:
			return TEXT("Explosive");
		default:
			return TEXT("Unknown");
		}
	}
};
