// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatConfig.h"
#include "CombatEvents.h"
#include "MarbleState.h"
#include "EnemyData.h"
#include "SceneManager.h"
#include "CombatManager.generated.h"

/**
 * 战斗管理器
 * 负责战斗流程控制、战斗状态管理、战斗事件调度
 * 
 * 设计理念：
 * - 事件驱动：使用事件管理器替代回合制
 * - 状态机：四阶段循环（准备、战斗、收集、结算）
 * - 模块解耦：通过事件总线与其他模块通信
 * 
 * 核心功能：
 * 1. 战斗流程控制（四阶段循环）
 * 2. 战斗状态管理（击杀数、战斗时间、玩家生命值）
 * 3. 战斗事件调度（发布和订阅战斗事件）
 */
UCLASS(Blueprintable)
class ECHOALCHEMIST_API UCombatManager : public UObject
{
	GENERATED_BODY()

public:
	// ========== 构造函数 ==========
	
	UCombatManager();

	// ========== 初始化 ==========
	
	/**
	 * 初始化战斗管理器
	 * @param InConfig 战斗配置
	 * @param InSceneManager 场景管理器
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Initialize(const FCombatConfig& InConfig, TScriptInterface<ISceneManager> InSceneManager);

	// ========== 战斗流程控制 ==========
	
	/**
	 * 开始战斗
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartCombat();

	/**
	 * 结束战斗
	 * @param bVictory 是否胜利
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndCombat(bool bVictory);

	/**
	 * 更新战斗状态
	 * @param DeltaTime 时间增量（秒）
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Tick(float DeltaTime);

	/**
	 * 切换到指定阶段
	 * @param NewPhase 新阶段
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TransitionToPhase(ECombatPhase NewPhase);

	// ========== 战斗状态查询 ==========
	
	/**
	 * 获取当前阶段
	 * @return 当前阶段
	 */
	UFUNCTION(BlueprintPure, Category = "Combat")
	ECombatPhase GetCurrentPhase() const { return CurrentPhase; }

	/**
	 * 是否正在战斗中
	 * @return 是否正在战斗中
	 */
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsInCombat() const { return bIsInCombat; }

	/**
	 * 获取击杀数
	 * @return 击杀数
	 */
	UFUNCTION(BlueprintPure, Category = "Combat")
	int32 GetKillCount() const { return KillCount; }

	/**
	 * 获取战斗时间
	 * @return 战斗时间（秒）
	 */
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetCombatTime() const { return CombatTime; }

	/**
	 * 获取玩家生命值
	 * @return 玩家生命值
	 */
	UFUNCTION(BlueprintPure, Category = "Combat")
	int32 GetPlayerHealth() const { return PlayerHealth; }

	// ========== 事件系统 ==========
	
	/**
	 * 发布战斗事件
	 * @param Event 战斗事件
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Events")
	void BroadcastEvent(const FCombatEvent& Event);

	/** 战斗事件委托 */
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnCombatEvent OnCombatEvent;

	// ========== 战斗统计 ==========
	
	/**
	 * 增加击杀数
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void IncrementKillCount();

	/**
	 * 应用玩家伤害
	 * @param Damage 伤害值
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyPlayerDamage(float Damage);

	/**
	 * 治疗玩家
	 * @param HealAmount 治疗量
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HealPlayer(float HealAmount);

protected:
	// ========== 战斗配置 ==========
	
	/** 战斗配置 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FCombatConfig Config;

	/** 场景管理器 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TScriptInterface<ISceneManager> SceneManager;

	// ========== 战斗状态 ==========
	
	/** 当前阶段 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	ECombatPhase CurrentPhase = ECombatPhase::Preparation;

	/** 是否正在战斗中 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsInCombat = false;

	/** 击杀数 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int32 KillCount = 0;

	/** 战斗时间（秒） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float CombatTime = 0.0f;

	/** 玩家生命值 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int32 PlayerHealth = 100;

	// ========== 内部方法 ==========
	
	/**
	 * 更新准备阶段
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdatePreparationPhase(float DeltaTime);

	/**
	 * 更新战斗阶段
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdateCombatPhase(float DeltaTime);

	/**
	 * 更新收集阶段
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdateCollectionPhase(float DeltaTime);

	/**
	 * 更新结算阶段
	 * @param DeltaTime 时间增量（秒）
	 */
	void UpdateSettlementPhase(float DeltaTime);

	/**
	 * 检查战斗结束条件
	 * @return 是否应该结束战斗
	 */
	bool CheckCombatEndConditions();
};
