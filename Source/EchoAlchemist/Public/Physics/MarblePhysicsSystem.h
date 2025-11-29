// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Physics/PhysicsSceneConfig.h"
#include "Physics/MarbleState.h"
#include "Physics/MarbleActorPool.h"
#include "MarblePhysicsSystem.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

/**
 * 魔力露珠物理系统
 * 
 * 这是物理系统的核心类，负责管理所有魔力露珠/魔药的物理模拟。
 * 支持炼金工作台和战斗场景的差异化物理规则。
 * 
 * 蓝图使用示例：
 * 
 * 1. 初始化场景
 *    ```
 *    // 创建物理系统实例
 *    PhysicsSystem = NewObject<UMarblePhysicsSystem>();
 *    
 *    // 创建场景配置
 *    FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(Min, Max);
 *    
 *    // 初始化场景
 *    PhysicsSystem->InitializeScene(Config);
 *    ```
 * 
 * 2. 发射魔力露珠
 *    ```
 *    // 创建发射参数
 *    FMarbleLaunchParams Params;
 *    Params.LaunchPosition = GetActorLocation();
 *    Params.LaunchDirection = GetActorForwardVector();
 *    Params.LaunchSpeed = 1000.0f;
 *    Params.Potency = 5.0f;
 *    
 *    // 发射
 *    FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);
 *    ```
 * 
 * 3. 更新物理（在Tick中调用）
 *    ```
 *    PhysicsSystem->Tick(DeltaTime);
 *    ```
 * 
 * 4. 获取魔力露珠状态
 *    ```
 *    TArray<FMarbleState> Marbles = PhysicsSystem->GetAllMarbles();
 *    for (const FMarbleState& Marble : Marbles)
 *    {
 *        // 更新视觉表现
 *        UpdateMarbleVisual(Marble.ID, Marble.Position, Marble.Velocity);
 *    }
 *    ```
 * 
 * 注意事项：
 * - 必须先调用 InitializeScene 才能使用其他功能
 * - 每帧必须调用 Tick 来更新物理状态
 * - 物理系统只负责逻辑，视觉表现由蓝图负责
 * - 碰撞检测由独立的 CollisionManager 负责（后续任务）
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API UMarblePhysicsSystem : public UObject
{
	GENERATED_BODY()

public:
	// ========== 初始化和清理 ==========
	
	/**
	 * 初始化物理场景
	 * 
	 * @param Config 场景配置（炼金工作台或战斗场景）
	 * 
	 * 注意事项：
	 * - 必须在使用其他功能前调用
	 * - 会清空所有现有的魔力露珠
	 * - 可以重复调用来切换场景
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|System")
	void InitializeScene(const FPhysicsSceneConfig& Config);

	/**
	 * 清理物理场景
	 * 
	 * 删除所有魔力露珠并重置系统状态。
	 * 通常在切换关卡或结束战斗时调用。
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|System")
	void CleanupScene();

	// ========== 魔力露珠管理 ==========
	
	/**
	 * 发射新的魔力露珠
	 * 
	 * @param Params 发射参数（位置、方向、速度等）
	 * @return 新创建的魔力露珠的唯一ID
	 * 
	 * 注意事项：
	 * - 返回的ID可用于后续查询或删除
	 * - 如果超过最大Actor数量，会自动使用粒子系统
	 * - 发射方向会自动归一化
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	FGuid LaunchMarble(const FMarbleLaunchParams& Params);

	/**
	 * 删除指定的魔力露珠
	 * 
	 * @param MarbleID 要删除的魔力露珠ID
	 * @return true=删除成功，false=ID不存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	bool RemoveMarble(const FGuid& MarbleID);

	/**
	 * 获取指定魔力露珠的状态
	 * 
	 * @param MarbleID 魔力露珠ID
	 * @param OutState 输出参数，存储魔力露珠状态
	 * @return true=获取成功，false=ID不存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	bool GetMarbleState(const FGuid& MarbleID, FMarbleState& OutState) const;

	/**
	 * 获取所有活跃的魔力露珠状态
	 * 
	 * @return 所有魔力露珠的状态数组
	 * 
	 * 注意事项：
	 * - 返回的是状态的拷贝，修改不会影响物理系统
	 * - 建议在Tick中调用来更新视觉表现
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	TArray<FMarbleState> GetAllMarbles() const;

	/**
	 * 获取活跃魔力露珠的数量
	 * 
	 * @return 当前活跃的魔力露珠数量
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Marble")
	int32 GetMarbleCount() const;

	/**
	 * 检查是否所有魔力露珠都已停止
	 * 
	 * @param SpeedThreshold 速度阈值（单位：cm/s，默认10）
	 * @return true=所有魔力露珠速度低于阈值，false=仍有魔力露珠在运动
	 * 
	 * 使用场景：
	 * - 炼金工作台：判断是否可以进入下一回合
	 * - 战斗场景：判断是否所有魔药都已耗尽
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Marble")
	bool AreAllMarblesStopped(float SpeedThreshold = 10.0f) const;

	// ========== 物理更新 ==========
	
	/**
	 * 物理系统Tick
	 * 
	 * @param DeltaTime 距离上次更新的时间（单位：秒）
	 * 
	 * 注意事项：
	 * - 必须每帧调用
	 * - 会更新所有魔力露珠的位置、速度
	 * - 会处理重力、边界、药效消耗等逻辑
	 * - 建议在Actor的Tick函数中调用
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|System")
	void Tick(float DeltaTime);

	// ========== 场景配置查询 ==========
	
	/**
	 * 获取当前场景配置
	 * 
	 * @return 当前的场景配置
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|System")
	FPhysicsSceneConfig GetSceneConfig() const { return SceneConfig; }

	/**
	 * 获取当前场景类型
	 * 
	 * @return 场景类型（炼金工作台或战斗场景）
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|System")
	TEnumAsByte<EPhysicsSceneType> GetSceneType() const { return SceneConfig.SceneType; }

	/**
	 * 检查系统是否已初始化
	 * 
	 * @return true=已初始化，false=未初始化
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|System")
	bool IsInitialized() const { return bIsInitialized; }

	// ========== 混合物理系统 ==========
	
	/**
	 * 初始化混合物理系统
	 * 
	 * @param World 世界对象（用于生成Actor）
	 * @param PreAllocateActorCount 预创建Actor数量
	 * 
	 * 注意事项：
	 * - 必须在InitializeScene之后调用
	 * - 如果不调用，所有魔力露珠都使用逻辑模拟
	 * - 调用后，第0-1代魔力露珠使用Actor，第2代及以上使用粒子
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|System")
	void InitializeHybridPhysics(UWorld* World, int32 PreAllocateActorCount = 20);

	/**
	 * 获取Actor对象池统计信息
	 * 
	 * @param OutTotalCount 总Actor数量
	 * @param OutAvailableCount 可用Actor数量
	 * @param OutInUseCount 正在使用的Actor数量
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|System|Debug")
	void GetActorPoolStatistics(int32& OutTotalCount, int32& OutAvailableCount, int32& OutInUseCount) const;

private:
	// ========== 内部状态 ==========
	
	/** 是否已初始化 */
	bool bIsInitialized = false;

	/** 场景配置 */
	FPhysicsSceneConfig SceneConfig;

	/** 活跃的魔力露珠映射表（ID -> State） */
	TMap<FGuid, FMarbleState> ActiveMarbles;

	/** Actor对象池 */
	UPROPERTY()
	UMarbleActorPool* ActorPool;

	/** Actor魔力露珠映射表（ID -> Actor） */
	TMap<FGuid, AMarbleActor*> MarbleActors;

	/** Niagara粒子系统组件 */
	UPROPERTY()
	UNiagaraComponent* ParticleSystem;

	/** 当前游戏时间（单位：秒） */
	float CurrentGameTime = 0.0f;

	// ========== 内部辅助函数 ==========
	
	/**
	 * 更新单个魔力露珠的物理状态
	 * 
	 * @param Marble 魔力露珠状态（引用，会被修改）
	 * @param DeltaTime 时间增量
	 */
	void UpdateMarblePhysics(FMarbleState& Marble, float DeltaTime);

	/**
	 * 应用重力
	 * 
	 * @param Marble 魔力露珠状态（引用，会被修改）
	 * @param DeltaTime 时间增量
	 */
	void ApplyGravity(FMarbleState& Marble, float DeltaTime);

	/**
	 * 处理边界碰撞
	 * 
	 * @param Marble 魔力露珠状态（引用，会被修改）
	 * @return true=魔力露珠仍然有效，false=应该被删除
	 */
	bool HandleBoundary(FMarbleState& Marble);

	/**
	 * 检查魔力露珠是否应该被删除
	 * 
	 * @param Marble 魔力露珠状态
	 * @return true=应该删除，false=保留
	 */
	bool ShouldRemoveMarble(const FMarbleState& Marble) const;

	/**
	 * 决定是否使用粒子系统
	 * 
	 * @param Generation 代数
	 * @return true=使用粒子，false=使用Actor
	 */
	bool ShouldUseParticle(int32 Generation) const;
};
