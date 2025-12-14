// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SpecialEffectData.generated.h"

/**
 * 特殊效果类型枚举
 * 
 * 定义游戏中所有可用的特殊物理效果类型。
 */
UENUM(BlueprintType)
enum class EEchoSpecialEffectType : uint8
{
	/** 无效果 */
	None UMETA(DisplayName = "None"),
	
	/** 引力奇点 - 对周围弹珠施加引力 */
	GravityWell UMETA(DisplayName = "Gravity Well"),
	
	/** 虫洞传送 - 将弹珠传送到另一个位置 */
	Wormhole UMETA(DisplayName = "Wormhole"),
	
	/** 弹珠分裂 - 将一个弹珠分裂成多个 */
	Split UMETA(DisplayName = "Split"),
	
	/** 速度加成 - 增加弹珠速度 */
	SpeedBoost UMETA(DisplayName = "Speed Boost"),
	
	/** 速度减缓 - 降低弹珠速度 */
	SpeedSlow UMETA(DisplayName = "Speed Slow"),
	
	/** 连锁触发 - 触发时发射次级弹珠 */
	ChainTrigger UMETA(DisplayName = "Chain Trigger")
};

/**
 * 引力奇点效果参数
 * 
 * 对周围弹珠施加引力，将其吸引到奇点位置。
 */
USTRUCT(BlueprintType)
struct FGravityWellParams
{
	GENERATED_BODY()

	/** 奇点位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector Position = FVector::ZeroVector;

	/** 引力强度（单位：cm/s²） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float GravityStrength = 1000.0f;

	/** 影响半径（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float EffectRadius = 500.0f;

	/** 持续时间（单位：秒，0表示永久） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Duration = 5.0f;

	/** 是否在弹珠到达中心时销毁 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool bDestroyOnReach = false;
};

/**
 * 虫洞传送效果参数
 * 
 * 将进入虫洞的弹珠传送到目标位置。
 */
USTRUCT(BlueprintType)
struct FWormholeParams
{
	GENERATED_BODY()

	/** 虫洞入口位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector EntrancePosition = FVector::ZeroVector;

	/** 虫洞出口位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector ExitPosition = FVector::ZeroVector;

	/** 入口半径（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float EntranceRadius = 50.0f;

	/** 出口速度倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float ExitSpeedMultiplier = 1.0f;

	/** 持续时间（单位：秒，0表示永久） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Duration = 10.0f;
};

/**
 * 弹珠分裂效果参数
 * 
 * 将一个弹珠分裂成多个子弹珠。
 */
USTRUCT(BlueprintType)
struct FSplitParams
{
	GENERATED_BODY()

	/** 分裂数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	int32 SplitCount = 2;

	/** 分裂角度（单位：度） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float SplitAngle = 30.0f;

	/** 子弹珠速度倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float SpeedMultiplier = 0.8f;

	/** 子弹珠药效倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float PotencyMultiplier = 0.5f;

	/** 子弹珠半径倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float RadiusMultiplier = 0.7f;
};

/**
 * 速度修改效果参数
 * 
 * 修改弹珠的速度（加速或减速）。
 */
USTRUCT(BlueprintType)
struct FEchoSpeedModifierParams
{
	GENERATED_BODY()

	/** 速度倍率（>1加速，<1减速） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float SpeedMultiplier = 1.5f;

	/** 是否立即应用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool bApplyImmediately = true;

	/** 持续时间（单位：秒，0表示瞬间） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Duration = 0.0f;
};

/**
 * 连锁触发效果参数
 * 
 * 触发时发射次级弹珠。
 */
USTRUCT(BlueprintType)
struct FChainTriggerParams
{
	GENERATED_BODY()

	/** 次级弹珠数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	int32 SecondaryCount = 3;

	/** 发射角度范围（单位：度） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float AngleRange = 60.0f;

	/** 次级弹珠速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float SecondarySpeed = 800.0f;

	/** 次级弹珠药效倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float PotencyMultiplier = 0.3f;

	/** 次级弹珠半径 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float SecondaryRadius = 5.0f;
};

/**
 * 特殊效果数据
 * 
 * 存储特殊效果的完整信息，包括类型和参数。
 */
USTRUCT(BlueprintType)
struct FSpecialEffectData
{
	GENERATED_BODY()

	/** 效果唯一标识符 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FGuid EffectID = FGuid::NewGuid();

	/** 效果类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	EEchoSpecialEffectType EffectType = EEchoSpecialEffectType::None;

	/** 效果创建时间（单位：秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float CreationTime = 0.0f;

	/** 是否激活 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool bIsActive = true;

	/** 引力奇点参数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Gravity")
	FGravityWellParams GravityWellParams;

	/** 虫洞传送参数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Wormhole")
	FWormholeParams WormholeParams;

	/** 弹珠分裂参数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Split")
	FSplitParams SplitParams;

	/** 速度修改参数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Speed")
	FEchoSpeedModifierParams SpeedModifierParams;

	/** 连锁触发参数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Chain")
	FChainTriggerParams ChainTriggerParams;

	/** 默认构造函数 */
	FSpecialEffectData()
	{
		EffectID = FGuid::NewGuid();
	}
};
