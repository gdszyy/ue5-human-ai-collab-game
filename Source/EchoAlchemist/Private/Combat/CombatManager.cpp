// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/CombatManager.h"

UCombatManager::UCombatManager()
{
	CurrentPhase = ECombatPhase::Preparation;
	bIsInCombat = false;
	KillCount = 0;
	CombatTime = 0.0f;
	PlayerHealth = 100;
}

void UCombatManager::Initialize(const FCombatConfig& InConfig, TScriptInterface<ISceneManager> InSceneManager)
{
	Config = InConfig;
	SceneManager = InSceneManager;
	PlayerHealth = Config.PlayerHealth;
	
	UE_LOG(LogTemp, Log, TEXT("CombatManager: Initialized with scene type: %s"), 
		SceneManager.GetInterface() ? *SceneManager->GetSceneType() : TEXT("None"));
}

void UCombatManager::StartCombat()
{
	if (bIsInCombat)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatManager: Combat already started"));
		return;
	}
	
	bIsInCombat = true;
	KillCount = 0;
	CombatTime = 0.0f;
	CurrentPhase = ECombatPhase::Preparation;
	
	// 发布战斗开始事件
	FCombatEvent Event;
	Event.EventType = ECombatEventType::CombatStarted;
	Event.Timestamp = FPlatformTime::Seconds();
	BroadcastEvent(Event);
	
	UE_LOG(LogTemp, Log, TEXT("CombatManager: Combat started"));
}

void UCombatManager::EndCombat(bool bVictory)
{
	if (!bIsInCombat)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatManager: Combat not started"));
		return;
	}
	
	bIsInCombat = false;
	CurrentPhase = ECombatPhase::Settlement;
	
	// 发布战斗结束事件
	FCombatEvent Event;
	Event.EventType = ECombatEventType::CombatEnded;
	Event.Timestamp = FPlatformTime::Seconds();
	Event.ExtraData.Add(TEXT("Victory"), bVictory ? 1.0f : 0.0f);
	Event.ExtraData.Add(TEXT("KillCount"), static_cast<float>(KillCount));
	Event.ExtraData.Add(TEXT("CombatTime"), CombatTime);
	BroadcastEvent(Event);
	
	UE_LOG(LogTemp, Log, TEXT("CombatManager: Combat ended. Victory: %s, Kills: %d, Time: %.1fs"),
		bVictory ? TEXT("Yes") : TEXT("No"), KillCount, CombatTime);
}

void UCombatManager::Tick(float DeltaTime)
{
	if (!bIsInCombat)
	{
		return;
	}
	
	// 更新战斗时间
	CombatTime += DeltaTime;
	
	// 根据当前阶段更新
	switch (CurrentPhase)
	{
	case ECombatPhase::Preparation:
		UpdatePreparationPhase(DeltaTime);
		break;
	case ECombatPhase::Combat:
		UpdateCombatPhase(DeltaTime);
		break;
	case ECombatPhase::Collection:
		UpdateCollectionPhase(DeltaTime);
		break;
	case ECombatPhase::Settlement:
		UpdateSettlementPhase(DeltaTime);
		break;
	}
	
	// 检查战斗结束条件
	if (CheckCombatEndConditions())
	{
		const bool bVictory = (KillCount >= Config.VictoryKillCount);
		EndCombat(bVictory);
	}
}

void UCombatManager::TransitionToPhase(ECombatPhase NewPhase)
{
	if (CurrentPhase == NewPhase)
	{
		return;
	}
	
	const ECombatPhase OldPhase = CurrentPhase;
	CurrentPhase = NewPhase;
	
	// 发布阶段切换事件
	FCombatEvent Event;
	Event.EventType = ECombatEventType::PhaseChanged;
	Event.Timestamp = FPlatformTime::Seconds();
	Event.ExtraData.Add(TEXT("OldPhase"), static_cast<float>(OldPhase));
	Event.ExtraData.Add(TEXT("NewPhase"), static_cast<float>(NewPhase));
	BroadcastEvent(Event);
	
	UE_LOG(LogTemp, Log, TEXT("CombatManager: Phase changed from %d to %d"), 
		static_cast<int32>(OldPhase), static_cast<int32>(NewPhase));
}

void UCombatManager::BroadcastEvent(const FCombatEvent& Event)
{
	OnCombatEvent.Broadcast(Event);
}

void UCombatManager::IncrementKillCount()
{
	KillCount++;
	
	UE_LOG(LogTemp, Log, TEXT("CombatManager: Kill count: %d / %d"), KillCount, Config.VictoryKillCount);
}

void UCombatManager::ApplyPlayerDamage(float Damage)
{
	PlayerHealth -= static_cast<int32>(Damage);
	PlayerHealth = FMath::Max(PlayerHealth, 0);
	
	// 发布玩家受伤事件
	FCombatEvent Event;
	Event.EventType = ECombatEventType::PlayerDamaged;
	Event.Timestamp = FPlatformTime::Seconds();
	Event.ExtraData.Add(TEXT("Damage"), Damage);
	Event.ExtraData.Add(TEXT("RemainingHealth"), static_cast<float>(PlayerHealth));
	BroadcastEvent(Event);
	
	UE_LOG(LogTemp, Log, TEXT("CombatManager: Player damaged. Damage: %.1f, Health: %d"), Damage, PlayerHealth);
}

void UCombatManager::HealPlayer(float HealAmount)
{
	PlayerHealth += static_cast<int32>(HealAmount);
	PlayerHealth = FMath::Min(PlayerHealth, Config.PlayerMaxHealth);
	
	// 发布玩家治疗事件
	FCombatEvent Event;
	Event.EventType = ECombatEventType::PlayerHealed;
	Event.Timestamp = FPlatformTime::Seconds();
	Event.ExtraData.Add(TEXT("HealAmount"), HealAmount);
	Event.ExtraData.Add(TEXT("CurrentHealth"), static_cast<float>(PlayerHealth));
	BroadcastEvent(Event);
	
	UE_LOG(LogTemp, Log, TEXT("CombatManager: Player healed. Heal: %.1f, Health: %d"), HealAmount, PlayerHealth);
}

void UCombatManager::UpdatePreparationPhase(float DeltaTime)
{
	// 准备阶段：玩家选择魔药和配方
	// 这里可以添加准备阶段的逻辑
	// 例如：等待玩家准备完成，然后切换到战斗阶段
}

void UCombatManager::UpdateCombatPhase(float DeltaTime)
{
	// 战斗阶段：魔药飞行、碰撞检测、伤害计算
	// 这里可以添加战斗阶段的逻辑
	// 例如：更新魔药状态、检测碰撞、计算伤害
}

void UCombatManager::UpdateCollectionPhase(float DeltaTime)
{
	// 收集阶段：收集掉落物
	// 这里可以添加收集阶段的逻辑
	// 例如：生成掉落物、等待玩家收集
}

void UCombatManager::UpdateSettlementPhase(float DeltaTime)
{
	// 结算阶段：显示战斗结果、奖励
	// 这里可以添加结算阶段的逻辑
	// 例如：计算奖励、显示结算界面
}

bool UCombatManager::CheckCombatEndConditions()
{
	// 检查胜利条件
	if (KillCount >= Config.VictoryKillCount)
	{
		return true;
	}
	
	// 检查失败条件
	if (PlayerHealth <= 0)
	{
		return true;
	}
	
	// 检查时间限制
	if (Config.bEnableTimeLimit && CombatTime >= Config.VictoryTime)
	{
		return true;
	}
	
	return false;
}
