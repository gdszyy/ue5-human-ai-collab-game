// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/CombatBlueprintLibrary.h"
#include "Combat/DamageCalculator.h"

FCombatConfig UCombatBlueprintLibrary::CreateCombatConfig(int32 Difficulty)
{
	if (Difficulty <= 3)
	{
		return FCombatConfig::CreateEasyConfig();
	}
	else if (Difficulty <= 7)
	{
		return FCombatConfig::CreateNormalConfig();
	}
	else
	{
		return FCombatConfig::CreateHardConfig();
	}
}

FPhysicsSceneConfig UCombatBlueprintLibrary::CreatePhysicsSceneConfig(EPhysicsSceneType SceneType)
{
	if (SceneType == EPhysicsSceneType::PhysicsScene_Workbench)
	{
		return USceneConfigFactory::CreateWorkbenchConfig();
	}
	else
	{
		// 使用默认边界
		return USceneConfigFactory::CreateCombatConfig(FVector(-500, -500, 0), FVector(500, 500, 1000));
	}
}

FMarbleState UCombatBlueprintLibrary::CreateMarbleState(
	EPotionType PotionType,
	FVector Position,
	FVector Velocity,
	float BaseDamage,
	float Potency
)
{
	FMarbleState Marble;
	Marble.ID = FGuid::NewGuid();
	Marble.PotionType = PotionType;
	Marble.Position = Position;
	Marble.Velocity = Velocity;
	Marble.BaseDamage = BaseDamage;
	Marble.Potency = Potency;
	Marble.MaxPotency = Potency;
	Marble.HitCount = 0;
	return Marble;
}

bool UCombatBlueprintLibrary::IsMarbleValid(const FMarbleState& Marble)
{
	return Marble.IsValid();
}

FEnemyData UCombatBlueprintLibrary::CreateEnemyData(
	EEnemyType EnemyType,
	FVector Position,
	float MaxHealth
)
{
	FEnemyData Enemy;
	Enemy.ID = FGuid::NewGuid();
	Enemy.EnemyType = EnemyType;
	Enemy.Position = Position;
	Enemy.Health = MaxHealth;
	Enemy.MaxHealth = MaxHealth;
	Enemy.State = EEnemyState::Idle;
	Enemy.bIsActive = true;
	
	// 根据敌人类型设置名称
	switch (EnemyType)
	{
	case EEnemyType::CrystalGolem:
		Enemy.Name = TEXT("Crystal Golem");
		break;
	case EEnemyType::EliteGolem:
		Enemy.Name = TEXT("Elite Golem");
		break;
	case EEnemyType::BossGolem:
		Enemy.Name = TEXT("Boss Golem");
		break;
	}
	
	return Enemy;
}

bool UCombatBlueprintLibrary::IsEnemyAlive(const FEnemyData& Enemy)
{
	return Enemy.IsAlive();
}

bool UCombatBlueprintLibrary::ApplyDamageToEnemy(FEnemyData& Enemy, float Damage)
{
	return Enemy.ApplyDamage(Damage);
}

FDamageInfo UCombatBlueprintLibrary::CalculateDamage(const FMarbleState& Marble, FGuid TargetID)
{
	return UDamageCalculator::CalculateDamage(Marble, TargetID);
}

FString UCombatBlueprintLibrary::GetDamageInfoString(const FDamageInfo& DamageInfo)
{
	return DamageInfo.ToString();
}

FGuid UCombatBlueprintLibrary::GenerateGuid()
{
	return FGuid::NewGuid();
}

bool UCombatBlueprintLibrary::IsGuidEqual(FGuid A, FGuid B)
{
	return A == B;
}

FString UCombatBlueprintLibrary::GuidToString(FGuid Guid)
{
	return Guid.ToString();
}
