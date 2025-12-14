// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SpecialEffects.generated.h"

/**
 * 特殊效果类型
 */
UENUM(BlueprintType)
enum class EEchoSpecialEffectType : uint8
{
	/** 无效果 */
	None UMETA(DisplayName = "None"),
	
	/** 引力奇点 - 吸引附近的魔力露珠 */
	GravitySingularity UMETA(DisplayName = "Gravity Singularity"),
	
	/** 虫洞传送 - 传送魔力露珠到另一个位置 */
	WormholeTeleport UMETA(DisplayName = "Wormhole Teleport"),
	
	/** 弹珠分裂 - 碰撞后分裂成多个子代魔药 */
	MarbleSplit UMETA(DisplayName = "Marble Split"),
	
	/** 速度加成 - 增加魔力露珠速度 */
	SpeedBoost UMETA(DisplayName = "Speed Boost"),
	
	/** 速度减缓 - 降低魔力露珠速度 */
	SpeedSlow UMETA(DisplayName = "Speed Slow"),
	
	/** 连锁触发 - 碰撞后发射次级魔药 */
	ChainReaction UMETA(DisplayName = "Chain Reaction")
};

/**
 * 引力奇点效果参数
 * 
 * 在指定位置产生引力场，吸引附近的魔力露珠。
 * 
 * 蓝图使用示例：
 * ```
 * FGravitySingularityParams Params;
 * Params.Position = GetActorLocation();
 * Params.Strength = 500.0f;  // 引力强度
 * Params.Radius = 300.0f;    // 影响半径
 * Params.Duration = 5.0f;    // 持续时间
 * ```
 */
USTRUCT(BlueprintType)
struct FGravitySingularityParams
{
	GENERATED_BODY()

	/** 奇点位置（世界坐标系） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	FVector Position = FVector::ZeroVector;

	/** 引力强度（单位：cm/s²） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity", meta = (ClampMin = "0.0"))
	float Strength = 500.0f;

	/** 影响半径（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity", meta = (ClampMin = "0.0"))
	float Radius = 300.0f;

	/** 持续时间（单位：秒，0表示永久） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity", meta = (ClampMin = "0.0"))
	float Duration = 0.0f;

	/** 创建时间（游戏时间） */
	UPROPERTY(BlueprintReadOnly, Category = "Gravity")
	float CreationTime = 0.0f;

	/** 唯一标识符 */
	UPROPERTY(BlueprintReadOnly, Category = "Gravity")
	FGuid ID;

	FGravitySingularityParams()
	{
		ID = FGuid::NewGuid();
	}

	/** 检查是否已过期 */
	FORCEINLINE bool IsExpired(float CurrentTime) const
	{
		return Duration > 0.0f && (CurrentTime - CreationTime) >= Duration;
	}
};

/**
 * 虫洞传送效果参数
 * 
 * 将进入入口的魔力露珠传送到出口位置。
 * 
 * 蓝图使用示例：
 * ```
 * FWormholeTeleportParams Params;
 * Params.EntryPosition = GetActorLocation();
 * Params.ExitPosition = TargetLocation;
 * Params.EntryRadius = 50.0f;
 * ```
 */
USTRUCT(BlueprintType)
struct FWormholeTeleportParams
{
	GENERATED_BODY()

	/** 入口位置（世界坐标系） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole")
	FVector EntryPosition = FVector::ZeroVector;

	/** 出口位置（世界坐标系） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole")
	FVector ExitPosition = FVector::ZeroVector;

	/** 入口半径（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole", meta = (ClampMin = "0.0"))
	float EntryRadius = 50.0f;

	/** 是否保持速度方向（true=保持，false=随机） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole")
	bool bPreserveVelocity = true;

	/** 速度倍率（传送后的速度缩放） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole", meta = (ClampMin = "0.0"))
	float VelocityMultiplier = 1.0f;

	/** 持续时间（单位：秒，0表示永久） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wormhole", meta = (ClampMin = "0.0"))
	float Duration = 0.0f;

	/** 创建时间（游戏时间） */
	UPROPERTY(BlueprintReadOnly, Category = "Wormhole")
	float CreationTime = 0.0f;

	/** 唯一标识符 */
	UPROPERTY(BlueprintReadOnly, Category = "Wormhole")
	FGuid ID;

	FWormholeTeleportParams()
	{
		ID = FGuid::NewGuid();
	}

	/** 检查是否已过期 */
	FORCEINLINE bool IsExpired(float CurrentTime) const
	{
		return Duration > 0.0f && (CurrentTime - CreationTime) >= Duration;
	}
};

/**
 * 弹珠分裂效果参数
 * 
 * 魔力露珠碰撞后分裂成多个子代魔药。
 * 
 * 蓝图使用示例：
 * ```
 * FMarbleSplitParams Params;
 * Params.SplitCount = 3;           // 分裂成3个
 * Params.SpeedMultiplier = 0.8f;   // 子代速度为父代的80%
 * Params.AngleSpread = 60.0f;      // 60度扩散角
 * ```
 */
USTRUCT(BlueprintType)
struct FMarbleSplitParams
{
	GENERATED_BODY()

	/** 分裂数量（子代魔药数量） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (ClampMin = "1", ClampMax = "10"))
	int32 SplitCount = 2;

	/** 速度倍率（子代速度相对父代的比例） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (ClampMin = "0.0"))
	float SpeedMultiplier = 0.8f;

	/** 角度扩散（单位：度，子代魔药的扩散角度） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float AngleSpread = 60.0f;

	/** 药效强度分配（子代药效相对父代的比例） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PotencyRatio = 0.5f;

	/** 最大分裂次数（防止无限分裂） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (ClampMin = "1"))
	int32 MaxSplitDepth = 2;
};

/**
 * 速度修改效果参数
 * 
 * 修改魔力露珠的速度（加速或减速）。
 * 
 * 蓝图使用示例：
 * ```
 * FEchoSpeedModifierParams Params;
 * Params.Multiplier = 1.5f;  // 加速50%
 * Params.Duration = 3.0f;    // 持续3秒
 * ```
 */
USTRUCT(BlueprintType)
struct FEchoSpeedModifierParams
{
	GENERATED_BODY()

	/** 速度倍率（1.0=不变，>1.0=加速，<1.0=减速） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (ClampMin = "0.0"))
	float Multiplier = 1.0f;

	/** 持续时间（单位：秒，0表示立即应用） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (ClampMin = "0.0"))
	float Duration = 0.0f;

	/** 是否平滑过渡（true=渐变，false=瞬间） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	bool bSmoothTransition = false;
};

/**
 * 连锁触发效果参数
 * 
 * 魔力露珠碰撞后发射次级魔药。
 * 
 * 蓝图使用示例：
 * ```
 * FChainReactionParams Params;
 * Params.ProjectileCount = 4;      // 发射4个次级魔药
 * Params.ProjectileSpeed = 800.0f; // 次级魔药速度
 * Params.AngleSpread = 90.0f;      // 90度扩散
 * ```
 */
USTRUCT(BlueprintType)
struct FChainReactionParams
{
	GENERATED_BODY()

	/** 次级魔药数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chain", meta = (ClampMin = "1", ClampMax = "20"))
	int32 ProjectileCount = 4;

	/** 次级魔药速度（单位：cm/s） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chain", meta = (ClampMin = "0.0"))
	float ProjectileSpeed = 800.0f;

	/** 角度扩散（单位：度） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chain", meta = (ClampMin = "0.0", ClampMax = "360.0"))
	float AngleSpread = 90.0f;

	/** 次级魔药药效强度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chain", meta = (ClampMin = "0.0"))
	float Potency = 1.0f;

	/** 次级魔药伤害倍率（相对主魔药） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chain", meta = (ClampMin = "0.0"))
	float DamageMultiplier = 0.5f;

	/** 最大连锁深度（防止无限连锁） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chain", meta = (ClampMin = "1"))
	int32 MaxChainDepth = 2;
};
