// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Combat/CombatManager.h"
#include "Combat/EnemyManager.h"
#include "Combat/DamageCalculator.h"
#include "Physics/MarblePhysicsSystem.h"
#include "Physics/CollisionManager.h"
#include "CombatPhysicsIntegrator.generated.h"

/**
 * 战斗物理集成器
 * 
 * 负责协调物理系统和战斗系统的交互，处理魔药与敌人的碰撞检测和伤害计算。
 * 
 * 设计理念：
 * - 集成层：作为物理系统和战斗系统之间的桥梁
 * - 事件驱动：通过碰撞事件触发伤害计算
 * - 生命周期管理：管理魔药的完整生命周期
 * - 性能优化：使用空间网格加速碰撞检测
 * 
 * 核心功能：
 * 1. 魔药与敌人的碰撞检测
 * 2. 碰撞后的伤害计算和应用
 * 3. 魔药状态更新（撞击次数、药效消耗）
 * 4. 协调物理系统和战斗系统的更新
 * 
 * 使用示例：
 * ```
 * // 创建集成器
 * UCombatPhysicsIntegrator* Integrator = NewObject<UCombatPhysicsIntegrator>();
 * 
 * // 初始化
 * Integrator->Initialize(CombatManager, EnemyManager, PhysicsSystem, CollisionManager);
 * 
 * // 发射魔药
 * FMarbleLaunchParams Params;
 * Params.LaunchPosition = PlayerPosition;
 * Params.LaunchDirection = AimDirection;
 * Params.LaunchSpeed = 1000.0f;
 * Params.Potency = 5.0f;
 * Integrator->LaunchMarble(Params);
 * 
 * // 每帧更新
 * Integrator->Tick(DeltaTime);
 * ```
 */
UCLASS(Blueprintable)
class ECHOALCHEMIST_API UCombatPhysicsIntegrator : public UObject
{
	GENERATED_BODY()

public:
	// ========== 构造函数 ==========
	
	UCombatPhysicsIntegrator();

	// ========== 初始化 ==========
	
	/**
	 * 初始化集成器
	 * @param InCombatManager 战斗管理器
	 * @param InEnemyManager 敌人管理器
	 * @param InPhysicsSystem 物理系统
	 * @param InCollisionManager 碰撞管理器
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Integration")
	void Initialize(
		UCombatManager* InCombatManager,
		UEnemyManager* InEnemyManager,
		UMarblePhysicsSystem* InPhysicsSystem,
		UCollisionManager* InCollisionManager
	);

	// ========== 魔药管理 ==========
	
	/**
	 * 发射魔药
	 * @param Params 发射参数
	 * @return 魔药ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Integration")
	FGuid LaunchMarble(const FMarbleLaunchParams& Params);

	/**
	 * 移除魔药
	 * @param MarbleID 魔药ID
	 * @return 是否成功移除
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Integration")
	bool RemoveMarble(FGuid MarbleID);

	/**
	 * 获取所有魔药状态
	 * @return 魔药状态列表
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Integration")
	TArray<FMarbleState> GetAllMarbles() const;

	/**
	 * 获取魔药数量
	 * @return 魔药数量
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Integration")
	int32 GetMarbleCount() const;

	// ========== 更新 ==========
	
	/**
	 * 更新集成器
	 * @param DeltaTime 时间增量（秒）
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Integration")
	void Tick(float DeltaTime);

	// ========== 碰撞处理 ==========
	
	/**
	 * 处理碰撞事件
	 * @param CollisionEvent 碰撞事件
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Integration")
	void HandleCollision(const FEchoCollisionEvent& CollisionEvent);

	// ========== 查询 ==========
	
	/**
	 * 检查是否所有魔药都已停止
	 * @param SpeedThreshold 速度阈值（cm/s）
	 * @return 是否所有魔药都已停止
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Integration")
	bool AreAllMarblesStopped(float SpeedThreshold = 10.0f) const;

protected:
	// ========== 系统引用 ==========
	
	/** 战斗管理器 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Integration")
	UCombatManager* CombatManager = nullptr;

	/** 敌人管理器 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Integration")
	UEnemyManager* EnemyManager = nullptr;

	/** 物理系统 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Integration")
	UMarblePhysicsSystem* PhysicsSystem = nullptr;

	/** 碰撞管理器 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Integration")
	UCollisionManager* CollisionManager = nullptr;

	// ========== 碰撞体映射 ==========
	
	/** 魔药ID到碰撞体ID的映射 */
	UPROPERTY()
	TMap<FGuid, FGuid> MarbleToCollisionBodyMap;

	/** 敌人ID到碰撞体ID的映射 */
	UPROPERTY()
	TMap<FGuid, FGuid> EnemyToCollisionBodyMap;

	/** 碰撞体ID到魔药ID的反向映射 */
	UPROPERTY()
	TMap<FGuid, FGuid> CollisionBodyToMarbleMap;

	/** 碰撞体ID到敌人ID的反向映射 */
	UPROPERTY()
	TMap<FGuid, FGuid> CollisionBodyToEnemyMap;

	// ========== 内部方法 ==========
	
	/**
	 * 更新物理系统
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdatePhysics(float DeltaTime);

	/**
	 * 更新碰撞体
	 */
	void UpdateCollisionBodies();

	/**
	 * 检测碰撞
	 */
	void DetectCollisions();

	/**
	 * 处理魔药与敌人的碰撞
	 * @param MarbleID 魔药ID
	 * @param EnemyID 敌人ID
	 */
	void HandleMarbleEnemyCollision(FGuid MarbleID, FGuid EnemyID);

	/**
	 * 注册魔药碰撞体
	 * @param MarbleID 魔药ID
	 * @param Position 位置
	 * @param Radius 半径
	 */
	void RegisterMarbleCollisionBody(FGuid MarbleID, FVector Position, float Radius);

	/**
	 * 注册敌人碰撞体
	 * @param EnemyID 敌人ID
	 * @param Position 位置
	 * @param Radius 半径
	 */
	void RegisterEnemyCollisionBody(FGuid EnemyID, FVector Position, float Radius);

	/**
	 * 更新魔药碰撞体位置
	 * @param MarbleID 魔药ID
	 * @param Position 新位置
	 */
	void UpdateMarbleCollisionBody(FGuid MarbleID, FVector Position);

	/**
	 * 更新敌人碰撞体位置
	 * @param EnemyID 敌人ID
	 * @param Position 新位置
	 */
	void UpdateEnemyCollisionBody(FGuid EnemyID, FVector Position);

	/**
	 * 移除魔药碰撞体
	 * @param MarbleID 魔药ID
	 */
	void RemoveMarbleCollisionBody(FGuid MarbleID);

	/**
	 * 移除敌人碰撞体
	 * @param EnemyID 敌人ID
	 */
	void RemoveEnemyCollisionBody(FGuid EnemyID);
};
