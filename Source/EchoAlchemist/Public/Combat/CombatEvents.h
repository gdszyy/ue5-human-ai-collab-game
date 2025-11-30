// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DamageInfo.h"
#include "CombatEvents.generated.h"

/**
 * 战斗事件类型
 * 定义战斗中可能发生的所有事件
 */
UENUM(BlueprintType)
enum class ECombatEventType : uint8
{
	// 战斗流程事件
	CombatStarted UMETA(DisplayName = "Combat Started"),
	CombatEnded UMETA(DisplayName = "Combat Ended"),
	PhaseChanged UMETA(DisplayName = "Phase Changed"),
	
	// 敌人事件
	EnemySpawned UMETA(DisplayName = "Enemy Spawned"),
	EnemyDamaged UMETA(DisplayName = "Enemy Damaged"),
	EnemyKilled UMETA(DisplayName = "Enemy Killed"),
	
	// 玩家事件
	PlayerDamaged UMETA(DisplayName = "Player Damaged"),
	PlayerHealed UMETA(DisplayName = "Player Healed"),
	
	// 魔药事件
	PotionLaunched UMETA(DisplayName = "Potion Launched"),
	PotionCollided UMETA(DisplayName = "Potion Collided"),
	PotionExpired UMETA(DisplayName = "Potion Expired")
};

/**
 * 战斗阶段
 * 定义战斗的四个阶段
 */
UENUM(BlueprintType)
enum class ECombatPhase : uint8
{
	Preparation UMETA(DisplayName = "Preparation"),  // 准备阶段
	Combat UMETA(DisplayName = "Combat"),            // 战斗阶段
	Collection UMETA(DisplayName = "Collection"),    // 收集阶段
	Settlement UMETA(DisplayName = "Settlement")     // 结算阶段
};

/**
 * 战斗事件数据
 * 描述一个战斗事件的所有信息
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FCombatEvent
{
	GENERATED_BODY()

	/** 事件类型 */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	ECombatEventType EventType = ECombatEventType::CombatStarted;

	/** 事件时间戳 */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	float Timestamp = 0.0f;

	/** 相关实体ID（如敌人ID、魔药ID等） */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	FGuid EntityID;

	/** 伤害信息（如果是伤害事件） */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	FDamageInfo DamageInfo;

	/** 额外数据（键值对） */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	TMap<FString, float> ExtraData;

	/** 获取事件类型名称 */
	FString GetEventTypeName() const
	{
		switch (EventType)
		{
		case ECombatEventType::CombatStarted: return TEXT("Combat Started");
		case ECombatEventType::CombatEnded: return TEXT("Combat Ended");
		case ECombatEventType::PhaseChanged: return TEXT("Phase Changed");
		case ECombatEventType::EnemySpawned: return TEXT("Enemy Spawned");
		case ECombatEventType::EnemyDamaged: return TEXT("Enemy Damaged");
		case ECombatEventType::EnemyKilled: return TEXT("Enemy Killed");
		case ECombatEventType::PlayerDamaged: return TEXT("Player Damaged");
		case ECombatEventType::PlayerHealed: return TEXT("Player Healed");
		case ECombatEventType::PotionLaunched: return TEXT("Potion Launched");
		case ECombatEventType::PotionCollided: return TEXT("Potion Collided");
		case ECombatEventType::PotionExpired: return TEXT("Potion Expired");
		default: return TEXT("Unknown");
		}
	}
};

// 战斗事件委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatEvent, const FCombatEvent&, Event);
