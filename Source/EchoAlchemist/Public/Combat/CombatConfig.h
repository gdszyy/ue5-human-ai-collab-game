// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatConfig.generated.h"

/**
 * 战斗配置
 * 定义战斗的全局参数
 * 
 * 设计理念：
 * - 数据驱动：所有战斗参数都可以通过配置调整
 * - 场景适配：支持下落式和环形两种战斗场景
 * - 平衡性：提供丰富的参数用于游戏平衡调整
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FCombatConfig
{
	GENERATED_BODY()

	// ========== 战斗场景类型 ==========
	
	/** 战斗场景类型（"Falling" 或 "Circular"） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	FString SceneType = TEXT("Falling");

	// ========== 敌人生成配置 ==========
	
	/** 最大敌人数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MaxEnemies = 20;

	/** 敌人生成间隔（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float EnemySpawnInterval = 2.0f;

	/** 初始敌人数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 InitialEnemyCount = 5;

	/** 是否使用程序化生成敌人 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	bool bUseProceduralGeneration = true;

	// ========== 玩家配置 ==========
	
	/** 玩家初始生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 PlayerHealth = 100;

	/** 玩家最大生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 PlayerMaxHealth = 100;

	/** 最大魔药槽位数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxPotionSlots = 5;

	// ========== 战斗结束条件 ==========
	
	/** 胜利所需击杀数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory")
	int32 VictoryKillCount = 50;

	/** 胜利所需时间（秒，0表示无时间限制） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory")
	float VictoryTime = 0.0f;

	/** 是否启用时间限制 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory")
	bool bEnableTimeLimit = false;

	// ========== 难度配置 ==========
	
	/** 难度等级（1-10） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	int32 DifficultyLevel = 1;

	/** 敌人生命值倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	float EnemyHealthMultiplier = 1.0f;

	/** 敌人伤害倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	float EnemyDamageMultiplier = 1.0f;

	/** 敌人生成速度倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	float EnemySpawnRateMultiplier = 1.0f;

	// ========== 奖励配置 ==========
	
	/** 基础经验值奖励 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 BaseExperienceReward = 100;

	/** 基础金币奖励 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 BaseGoldReward = 50;

	// ========== 工厂方法 ==========
	
	/**
	 * 创建简单战斗配置
	 * @return 简单战斗配置
	 */
	static FCombatConfig CreateEasyConfig()
	{
		FCombatConfig Config;
		Config.SceneType = TEXT("Falling");
		Config.MaxEnemies = 10;
		Config.EnemySpawnInterval = 3.0f;
		Config.InitialEnemyCount = 3;
		Config.bUseProceduralGeneration = false;
		Config.PlayerHealth = 100;
		Config.PlayerMaxHealth = 100;
		Config.MaxPotionSlots = 5;
		Config.VictoryKillCount = 20;
		Config.VictoryTime = 0.0f;
		Config.bEnableTimeLimit = false;
		Config.DifficultyLevel = 1;
		Config.EnemyHealthMultiplier = 0.8f;
		Config.EnemyDamageMultiplier = 0.8f;
		Config.EnemySpawnRateMultiplier = 0.8f;
		Config.BaseExperienceReward = 100;
		Config.BaseGoldReward = 50;
		return Config;
	}

	/**
	 * 创建普通战斗配置
	 * @return 普通战斗配置
	 */
	static FCombatConfig CreateNormalConfig()
	{
		FCombatConfig Config;
		Config.SceneType = TEXT("Falling");
		Config.MaxEnemies = 20;
		Config.EnemySpawnInterval = 2.0f;
		Config.InitialEnemyCount = 5;
		Config.bUseProceduralGeneration = true;
		Config.PlayerHealth = 100;
		Config.PlayerMaxHealth = 100;
		Config.MaxPotionSlots = 5;
		Config.VictoryKillCount = 50;
		Config.VictoryTime = 0.0f;
		Config.bEnableTimeLimit = false;
		Config.DifficultyLevel = 5;
		Config.EnemyHealthMultiplier = 1.0f;
		Config.EnemyDamageMultiplier = 1.0f;
		Config.EnemySpawnRateMultiplier = 1.0f;
		Config.BaseExperienceReward = 200;
		Config.BaseGoldReward = 100;
		return Config;
	}

	/**
	 * 创建困难战斗配置
	 * @return 困难战斗配置
	 */
	static FCombatConfig CreateHardConfig()
	{
		FCombatConfig Config;
		Config.SceneType = TEXT("Falling");
		Config.MaxEnemies = 30;
		Config.EnemySpawnInterval = 1.5f;
		Config.InitialEnemyCount = 8;
		Config.bUseProceduralGeneration = true;
		Config.PlayerHealth = 100;
		Config.PlayerMaxHealth = 100;
		Config.MaxPotionSlots = 5;
		Config.VictoryKillCount = 100;
		Config.VictoryTime = 300.0f;
		Config.bEnableTimeLimit = true;
		Config.DifficultyLevel = 10;
		Config.EnemyHealthMultiplier = 1.5f;
		Config.EnemyDamageMultiplier = 1.5f;
		Config.EnemySpawnRateMultiplier = 1.5f;
		Config.BaseExperienceReward = 500;
		Config.BaseGoldReward = 250;
		return Config;
	}
};
