// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MarbleState.generated.h"

/**
 * 魔药类型
 * 定义不同的魔药类型及其特性
 */
UENUM(BlueprintType)
enum class EPotionType : uint8
{
	/** 弹射药剂 - 伤害递增 */
	Ricochet UMETA(DisplayName = "Ricochet"),
	
	/** 穿透药剂 - 穿透敌人 */
	Piercing UMETA(DisplayName = "Piercing"),
	
	/** 爆炸药剂 - 范围伤害 */
	Explosive UMETA(DisplayName = "Explosive")
};

/**
 * 魔力露珠/魔药状态
 * 
 * 这个结构体存储单个魔力露珠或魔药的完整物理状态。
 * 包括位置、速度、生命周期等所有必要信息。
 * 
 * 蓝图使用示例：
 * 1. 通过 MarblePhysicsSystem 的 GetMarbleState 函数获取状态
 * 2. 读取 Position、Velocity 等属性用于视觉表现
 * 3. 监听 Potency 变化来更新UI或触发特效
 * 
 * 注意事项：
 * - ID 是唯一标识符，用于追踪特定的魔力露珠
 * - Potency 仅在战斗场景中有效（炼金工作台场景忽略此值）
 * - Generation 用于实现分级降级策略（第0代用Actor，第2代及以上用粒子）
 */
USTRUCT(BlueprintType)
struct FMarbleState
{
	GENERATED_BODY()

	/** 唯一标识符 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FGuid ID;

	// ========== 物理状态 ==========
	
	/** 当前位置（世界坐标系，单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	FVector Position = FVector::ZeroVector;

	/** 当前速度（单位：cm/s） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	FVector Velocity = FVector::ZeroVector;

	/** 半径（单位：cm，用于碰撞检测） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.1"))
	float Radius = 10.0f;

	/** 质量（单位：g，影响碰撞后的速度变化） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.1"))
	float Mass = 100.0f;

	// ========== 魔药类型（仅战斗场景） ==========
	
	/** 魔药类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	EPotionType PotionType = EPotionType::Ricochet;
	
	// ========== 生命周期（仅战斗场景） ==========
	
	/** 当前药效强度（每次碰撞消耗，耗尽后删除） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.0"))
	float Potency = 1.0f;

	/** 最大药效强度（由持续药剂决定） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.0"))
	float MaxPotency = 1.0f;

	// ========== 伤害递增（仅战斗场景，弹射药剂专用） ==========
	
	/** 碰撞次数（用于计算弹射药剂的伤害递增） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 HitCount = 0;

	// ========== 分级降级 ==========
	
	/** 代数（第0代为玩家发射，第1代为分裂产生，第2代及以上用粒子优化） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization")
	int32 Generation = 0;

	/** 是否使用粒子系统实现（true=粒子，false=Actor） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization")
	bool bUseParticle = false;

	// ========== 时间戳 ==========
	
	/** 创建时间（游戏时间，单位：秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float CreationTime = 0.0f;

	/** 上次更新时间（游戏时间，单位：秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float LastUpdateTime = 0.0f;

	/** 构造函数 - 生成唯一ID */
	FMarbleState()
	{
		ID = FGuid::NewGuid();
	}

	/**
	 * 检查魔力露珠是否仍然有效
	 * 
	 * @return true=有效，false=应该被删除
	 * 
	 * 判断条件：
	 * - 战斗场景：Potency > 0
	 * - 炼金工作台：始终有效（由边界行为决定删除）
	 */
	FORCEINLINE bool IsValid() const
	{
		return Potency > 0.0f;
	}

	/**
	 * 消耗药效强度
	 * 
	 * @param Amount 消耗量（默认1.0）
	 * @return 消耗后的剩余药效强度
	 */
	FORCEINLINE float ConsumePotency(float Amount = 1.0f)
	{
		Potency = FMath::Max(0.0f, Potency - Amount);
		return Potency;
	}

	/**
	 * 增加碰撞次数
	 * 
	 * @return 增加后的碰撞次数
	 */
	FORCEINLINE int32 IncrementHitCount()
	{
		return ++HitCount;
	}
};

/**
 * 魔力露珠发射参数
 * 
 * 用于创建新的魔力露珠或魔药时传递初始参数。
 * 
 * 蓝图使用示例：
 * 1. 创建一个 MarbleLaunchParams 变量
 * 2. 设置发射位置、方向、速度
 * 3. 设置药效强度（战斗场景）
 * 4. 调用 MarblePhysicsSystem 的 LaunchMarble 函数
 * 
 * 注意事项：
 * - LaunchDirection 会自动归一化
 * - Potency 仅在战斗场景中有效
 * - Generation 用于分裂效果（子代魔药的代数+1）
 */
USTRUCT(BlueprintType)
struct FMarbleLaunchParams
{
	GENERATED_BODY()

	// ========== 发射参数 ==========
	
	/** 发射位置（世界坐标系） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch")
	FVector LaunchPosition = FVector::ZeroVector;

	/** 发射方向（会自动归一化） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch")
	FVector LaunchDirection = FVector(1, 0, 0);

	/** 发射速度（单位：cm/s） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch", meta = (ClampMin = "0.0"))
	float LaunchSpeed = 1000.0f;

	// ========== 物理属性 ==========
	
	/** 半径（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.1"))
	float Radius = 10.0f;

	/** 质量（单位：g） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (ClampMin = "0.1"))
	float Mass = 100.0f;

	// ========== 生命周期（仅战斗场景） ==========
	
	/** 初始药效强度（由配方系统决定） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.0"))
	float Potency = 1.0f;

	// ========== 分级降级 ==========
	
	/** 代数（0=玩家发射，1=第一次分裂，2+=粒子优化） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization", meta = (ClampMin = "0"))
	int32 Generation = 0;

	/** 构造函数 - 设置默认值 */
	FMarbleLaunchParams()
	{
	}
};
