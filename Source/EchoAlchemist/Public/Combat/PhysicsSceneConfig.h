// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsSceneConfig.generated.h"

/**
 * 场景类型
 * 定义不同的游戏模式
 */
UENUM(BlueprintType)
enum class EPhysicsSceneType : uint8
{
	Workbench UMETA(DisplayName = "Alchemy Workbench"),  // 炼金工作台
	Combat UMETA(DisplayName = "Combat Scene")            // 战斗场景
};

/**
 * 边界行为
 * 定义魔力露珠/魔药离开边界时的行为
 */
UENUM(BlueprintType)
enum class EBoundaryBehavior : uint8
{
	Delete UMETA(DisplayName = "Delete"),  // 离开边界删除
	Bounce UMETA(DisplayName = "Bounce"),  // 碰到边界反弹
	None UMETA(DisplayName = "None")       // 无边界
};

/**
 * 碰撞体形状
 * 定义碰撞检测使用的形状类型
 */
UENUM(BlueprintType)
enum class ECollisionShapeType : uint8
{
	Circle UMETA(DisplayName = "Circle"),        // 圆形
	Rectangle UMETA(DisplayName = "Rectangle")   // 矩形
};

/**
 * 物理场景配置
 * 定义不同游戏模式的物理规则
 * 
 * 设计理念：
 * - 场景差异化：通过配置实现炼金工作台和战斗场景的物理差异
 * - 数据驱动：所有物理参数都可以通过配置调整
 * - 性能可控：提供性能相关的配置选项
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FPhysicsSceneConfig
{
	GENERATED_BODY()

	// ========== 场景类型 ==========
	
	/** 场景类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	EPhysicsSceneType SceneType = EPhysicsSceneType::Combat;

	// ========== 重力配置 ==========
	
	/** 是否启用重力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	bool bEnableGravity = false;

	/** 重力方向（归一化向量） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	FVector GravityDirection = FVector(0, 0, -1);

	/** 重力强度（cm/s²） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float GravityStrength = 980.0f;

	// ========== 边界配置 ==========
	
	/** 是否有边界 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	bool bHasBoundary = false;

	/** 边界盒（世界坐标） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	FBox BoundaryBox = FBox(FVector(-400, -300, -100), FVector(400, 300, 100));

	/** 边界行为 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	EBoundaryBehavior BoundaryBehavior = EBoundaryBehavior::None;

	// ========== 碰撞配置 ==========
	
	/** 魔力露珠/魔药的碰撞形状 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ECollisionShapeType MarbleShape = ECollisionShapeType::Circle;

	/** 魔力露珠/魔药的碰撞半径（cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float MarbleRadius = 10.0f;

	// ========== 生命周期配置 ==========
	
	/** 是否使用药效强度系统（战斗场景） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle")
	bool bUsePotencySystem = false;

	/** 是否使用以太能量系统（炼金工作台） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle")
	bool bUseEtherEnergySystem = false;

	// ========== 性能配置 ==========
	
	/** 最大Actor魔力露珠/魔药数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	int32 MaxActorMarbles = 10;

	/** 是否启用粒子优化（超过MaxActorMarbles后使用Niagara粒子） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	bool bEnableParticleOptimization = false;

	/** 空间网格单元大小（cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	float SpatialGridCellSize = 100.0f;

	// ========== 工厂方法 ==========
	
	/**
	 * 创建炼金工作台场景配置
	 * @return 炼金工作台场景配置
	 */
	static FPhysicsSceneConfig CreateWorkbenchConfig()
	{
		FPhysicsSceneConfig Config;
		Config.SceneType = EPhysicsSceneType::Workbench;
		Config.bEnableGravity = true;
		Config.GravityDirection = FVector(0, 0, -1);
		Config.GravityStrength = 980.0f;
		Config.bHasBoundary = true;
		Config.BoundaryBox = FBox(FVector(-400, -300, -100), FVector(400, 300, 100));
		Config.BoundaryBehavior = EBoundaryBehavior::Bounce;
		Config.MarbleShape = ECollisionShapeType::Circle;
		Config.MarbleRadius = 10.0f;
		Config.bUsePotencySystem = false;
		Config.bUseEtherEnergySystem = true;
		Config.MaxActorMarbles = 10;
		Config.bEnableParticleOptimization = false;
		Config.SpatialGridCellSize = 100.0f;
		return Config;
	}

	/**
	 * 创建战斗场景配置
	 * @return 战斗场景配置
	 */
	static FPhysicsSceneConfig CreateCombatConfig()
	{
		FPhysicsSceneConfig Config;
		Config.SceneType = EPhysicsSceneType::Combat;
		Config.bEnableGravity = false;
		Config.bHasBoundary = false;
		Config.BoundaryBehavior = EBoundaryBehavior::None;
		Config.MarbleShape = ECollisionShapeType::Circle;
		Config.MarbleRadius = 10.0f;
		Config.bUsePotencySystem = true;
		Config.bUseEtherEnergySystem = false;
		Config.MaxActorMarbles = 10;
		Config.bEnableParticleOptimization = true;
		Config.SpatialGridCellSize = 100.0f;
		return Config;
	}
};
