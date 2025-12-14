// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CollisionShape.h"
#include "Combat/CombatSceneTypes.h"
#include "PhysicsSceneConfig.generated.h"

/**
 * 物理场景类型
 * 用于区分炼金工作台和战斗场景的不同物理规则
 */
UENUM(BlueprintType)
enum class EPhysicsSceneType : uint8
{
	/** 炼金工作台场景 - 有重力，高精度物理 */
	PhysicsScene_Workbench UMETA(DisplayName = "Workbench"),
	
	/** 战斗场景 - 无重力，支持大量魔药 */
	PhysicsScene_Combat UMETA(DisplayName = "Combat")
};

/**
 * 边界行为类型
 * 定义魔力露珠/魔药碰到边界时的行为
 */
UENUM(BlueprintType)
enum class EBoundaryBehavior : uint8
{
	/** 离开边界时删除 */
	Boundary_Delete UMETA(DisplayName = "Delete"),
	
	/** 碰到边界时反弹 */
	Boundary_Bounce UMETA(DisplayName = "Bounce"),
	
	/** 无边界限制 */
	Boundary_None UMETA(DisplayName = "None")
};

/**
 * 物理场景配置
 * 
 * 这个结构体定义了物理系统在不同场景下的行为规则。
 * 通过配置不同的参数，可以实现炼金工作台和战斗场景的差异化物理表现。
 * 
 * 蓝图使用示例：
 * 1. 创建一个 PhysicsSceneConfig 变量
 * 2. 设置场景类型（Workbench 或 Combat）
 * 3. 配置重力、边界等参数
 * 4. 传递给 MarblePhysicsSystem 的 InitializeScene 函数
 */
USTRUCT(BlueprintType)
struct FPhysicsSceneConfig
{
	GENERATED_BODY()

	/** 场景类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	EPhysicsSceneType SceneType = EPhysicsSceneType::PhysicsScene_Combat;

	// ========== 重力配置 ==========
	
	/** 是否启用重力（炼金工作台：true，战斗场景：false） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	bool bEnableGravity = false;

	/** 重力方向（归一化向量，默认向下） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	FVector GravityDirection = FVector(0, 0, -1);

	/** 重力强度（cm/s²，默认980对应现实重力） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity", meta = (ClampMin = "0.0"))
	float GravityStrength = 980.0f;

	// ========== 边界配置 ==========
	
	/** 是否有边界限制 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	bool bHasBoundary = false;

	/** 边界盒（世界坐标系） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	FBox BoundaryBox = FBox(FVector(-1000, -1000, -1000), FVector(1000, 1000, 1000));
	
	/** 边界最小点（世界坐标系） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	FVector BoundsMin = FVector(-1000, -1000, -1000);
	
	/** 边界最大点（世界坐标系） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	FVector BoundsMax = FVector(1000, 1000, 1000);

	/** 边界行为（删除、反弹或无） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
	EBoundaryBehavior BoundaryBehavior = EBoundaryBehavior::Boundary_None;

	// ========== 碰撞配置 ==========
	
	/** 魔力露珠/魔药的碰撞体形状 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	EEchoCollisionShapeType MarbleShape = EEchoCollisionShapeType::Circle;

	// ========== 生命周期配置 ==========
	
	/** 是否使用药效强度系统（战斗场景：true，炼金工作台：false） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle")
	bool bUsePotencySystem = false;

	// ========== 性能配置 ==========
	
	/** 最大Actor魔力露珠数量（超过此数量将使用粒子系统优化） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance", meta = (ClampMin = "1"))
	int32 MaxActorMarbles = 10;

	/** 是否启用粒子系统优化 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
	bool bEnableParticleOptimization = false;

	/** 构造函数 - 设置默认值 */
	FPhysicsSceneConfig()
	{
	}
};

/**
 * 场景配置工厂
 * 
 * 提供快速创建预设场景配置的静态函数。
 * 
 * 蓝图使用示例：
 * - 调用 CreateWorkbenchConfig 创建炼金工作台配置
 * - 调用 CreateCombatConfig 创建战斗场景配置
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API USceneConfigFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 创建炼金工作台场景配置
	 * 
	 * @return 炼金工作台的物理场景配置
	 * 
	 * 特点：
	 * - 启用重力
	 * - 高精度物理
	 * - 边界行为为删除
	 * - 不使用药效强度系统
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Config")
	static FPhysicsSceneConfig CreateWorkbenchConfig();

	/**
	 * 创建战斗场景配置
	 * 
	 * @param BoundaryMin 边界盒最小点（世界坐标）
	 * @param BoundaryMax 边界盒最大点（世界坐标）
	 * @return 战斗场景的物理场景配置
	 * 
	 * 特点：
	 * - 无重力
	 * - 支持大量魔药
	 * - 边界行为为反弹
	 * - 使用药效强度系统
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Config")
	static FPhysicsSceneConfig CreateCombatConfig(FVector BoundaryMin, FVector BoundaryMax);
};
