// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MarbleState.generated.h"

/**
 * 魔药类型
 * 定义不同的魔药基底
 */
UENUM(BlueprintType)
enum class EPotionType : uint8
{
	Ricochet UMETA(DisplayName = "Ricochet Potion"),    // 弹射药剂（原烈焰酊剂）
	Piercing UMETA(DisplayName = "Piercing Essence"),   // 穿刺精粹
	Explosive UMETA(DisplayName = "Explosive Elixir")   // 爆裂灵液
};

/**
 * 物理实现方式
 * 定义魔力露珠/魔药的物理实现方式
 */
UENUM(BlueprintType)
enum class EMarbleImplementation : uint8
{
	Actor UMETA(DisplayName = "Actor"),        // 由AMarbleActor实现
	Particle UMETA(DisplayName = "Particle")   // 由Niagara粒子实现
};

/**
 * 魔力露珠/魔药状态
 * 描述单个魔力露珠/魔药的所有状态
 * 
 * 设计理念：
 * - 数据驱动：所有状态都存储在数据结构中
 * - 场景适配：同时支持炼金工作台和战斗场景
 * - 性能优化：支持Actor和粒子两种实现方式
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FMarbleState
{
	GENERATED_BODY()

	// ========== 唯一标识符 ==========
	
	/** 唯一标识符 */
	UPROPERTY(BlueprintReadWrite, Category = "Identity")
	FGuid ID;

	// ========== 物理状态 ==========
	
	/** 位置（世界坐标） */
	UPROPERTY(BlueprintReadWrite, Category = "Physics")
	FVector Position = FVector::ZeroVector;

	/** 速度（cm/s） */
	UPROPERTY(BlueprintReadWrite, Category = "Physics")
	FVector Velocity = FVector::ZeroVector;

	/** 半径（cm） */
	UPROPERTY(BlueprintReadWrite, Category = "Physics")
	float Radius = 10.0f;

	// ========== 生命周期（战斗场景） ==========
	
	/** 药效强度（战斗场景） */
	UPROPERTY(BlueprintReadWrite, Category = "Lifecycle")
	float Potency = 0.0f;

	/** 最大药效强度（战斗场景） */
	UPROPERTY(BlueprintReadWrite, Category = "Lifecycle")
	float MaxPotency = 0.0f;

	// ========== 以太能量（炼金工作台） ==========
	
	/** 以太能量（炼金工作台） */
	UPROPERTY(BlueprintReadWrite, Category = "Lifecycle")
	float EtherEnergy = 0.0f;

	// ========== 伤害递增（弹射药剂） ==========
	
	/** 撞击次数（用于计算弹射药剂的伤害递增） */
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	int32 HitCount = 0;

	// ========== 性能与实现 ==========
	
	/** 代数（0为玩家直接发射，1为第一代子弹，以此类推） */
	UPROPERTY(BlueprintReadWrite, Category = "Performance")
	int32 Generation = 0;

	/** 物理实现方式 */
	UPROPERTY(BlueprintReadWrite, Category = "Performance")
	EMarbleImplementation Implementation = EMarbleImplementation::Actor;

	// ========== 关联数据 ==========
	
	/** 魔药类型 */
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	EPotionType PotionType = EPotionType::Ricochet;

	/** 基础伤害 */
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	float BaseDamage = 10.0f;

	/** 是否激活 */
	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool bIsActive = true;

	// ========== 构造函数 ==========
	
	FMarbleState()
	{
		ID = FGuid::NewGuid();
	}

	// ========== 辅助方法 ==========
	
	/**
	 * 检查魔力露珠/魔药是否还有效
	 * @return 是否有效
	 */
	bool IsValid() const
	{
		if (!bIsActive)
		{
			return false;
		}

		// 战斗场景：检查药效强度
		if (Potency > 0.0f || MaxPotency > 0.0f)
		{
			return Potency > 0.0f;
		}

		// 炼金工作台：检查以太能量
		if (EtherEnergy > 0.0f)
		{
			return true;
		}

		// 默认有效
		return true;
	}

	/**
	 * 获取药效强度需求（根据魔药类型）
	 * @return 药效强度需求
	 */
	float GetPotencyRequired() const
	{
		switch (PotionType)
		{
		case EPotionType::Ricochet:
			return 1.0f;
		case EPotionType::Piercing:
			return 1.5f;
		case EPotionType::Explosive:
			return 3.0f;
		default:
			return 1.0f;
		}
	}

	/**
	 * 消耗药效强度
	 * @param Amount 消耗量
	 * @return 实际消耗量
	 */
	float ConsumePotency(float Amount)
	{
		const float ActualAmount = FMath::Min(Amount, Potency);
		Potency -= ActualAmount;
		return ActualAmount;
	}

	/**
	 * 增加撞击次数
	 */
	void IncrementHitCount()
	{
		HitCount++;
	}
};
