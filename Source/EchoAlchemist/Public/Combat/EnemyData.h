// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Physics/PhysicsSceneConfig.h"
#include "Physics/CollisionShape.h"
#include "EnemyData.generated.h"

/**
 * 敌人类型
 * 定义不同的敌人类型
 */
UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	CrystalGolem UMETA(DisplayName = "Crystal Golem"),  // 晶石魔像（普通敌人）
	EliteGolem UMETA(DisplayName = "Elite Golem"),      // 精英魔像
	BossGolem UMETA(DisplayName = "Boss Golem")         // Boss魔像
};

/**
 * 敌人状态
 * 定义敌人的行为状态
 */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle"),          // 空闲
	Moving UMETA(DisplayName = "Moving"),      // 移动
	Attacking UMETA(DisplayName = "Attacking"), // 攻击
	Dying UMETA(DisplayName = "Dying"),        // 死亡中
	Dead UMETA(DisplayName = "Dead")           // 已死亡
};

/**
 * 敌人数据
 * 描述单个敌人的所有状态
 * 
 * 设计理念：
 * - 数据驱动：所有状态都存储在数据结构中
 * - 程序化生成：支持从VOI-11怪兽生成系统生成的属性
 * - 手工设计：支持从DataTable加载预定义的敌人数据
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FEnemyData
{
	GENERATED_BODY()

	// ========== 唯一标识符 ==========
	
	/** 唯一标识符 */
	UPROPERTY(BlueprintReadWrite, Category = "Identity")
	FGuid ID;

	/** 敌人名称 */
	UPROPERTY(BlueprintReadWrite, Category = "Identity")
	FString Name = TEXT("Crystal Golem");

	// ========== 敌人类型 ==========
	
	/** 敌人类型 */
	UPROPERTY(BlueprintReadWrite, Category = "Type")
	EEnemyType EnemyType = EEnemyType::CrystalGolem;

	// ========== 位置和移动 ==========
	
	/** 位置（世界坐标） */
	UPROPERTY(BlueprintReadWrite, Category = "Transform")
	FVector Position = FVector::ZeroVector;

	/** 速度（cm/s） */
	UPROPERTY(BlueprintReadWrite, Category = "Transform")
	FVector Velocity = FVector::ZeroVector;

	/** 移动速度（cm/s） */
	UPROPERTY(BlueprintReadWrite, Category = "Transform")
	float MoveSpeed = 100.0f;

	// ========== 生命值 ==========
	
	/** 当前生命值 */
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;

	/** 最大生命值 */
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	// ========== 状态 ==========
	
	/** 当前状态 */
	UPROPERTY(BlueprintReadWrite, Category = "State")
	EEnemyState State = EEnemyState::Idle;

	/** 是否激活 */
	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool bIsActive = true;

	// ========== 碰撞形状 ==========
	
	/** 碰撞形状 */
	UPROPERTY(BlueprintReadWrite, Category = "Collision")
	EEchoCollisionShapeType CollisionShape = EEchoCollisionShapeType::Circle;

	/** 碰撞半径（cm，用于圆形碰撞） */
	UPROPERTY(BlueprintReadWrite, Category = "Collision")
	float CollisionRadius = 50.0f;

	/** 碰撞盒大小（cm，用于矩形碰撞） */
	UPROPERTY(BlueprintReadWrite, Category = "Collision")
	FVector CollisionBoxExtent = FVector(50, 50, 50);

	// ========== 程序化生成属性（来自VOI-11怪兽生成系统） ==========
	
	/** 生态属性（如：栖息地偏好、活动时间等） */
	UPROPERTY(BlueprintReadWrite, Category = "PCG")
	TMap<FString, float> EcologicalAttributes;

	/** 战斗属性（如：攻击力、防御力、速度等） */
	UPROPERTY(BlueprintReadWrite, Category = "PCG")
	TMap<FString, float> CombatAttributes;

	// ========== 构造函数 ==========
	
	FEnemyData()
	{
		ID = FGuid::NewGuid();
	}

	// ========== 辅助方法 ==========
	
	/**
	 * 检查敌人是否存活
	 * @return 是否存活
	 */
	bool IsAlive() const
	{
		return bIsActive && State != EEnemyState::Dead && Health > 0.0f;
	}

	/**
	 * 检查敌人是否死亡
	 * @return 是否死亡
	 */
	bool IsDead() const
	{
		return !bIsActive || State == EEnemyState::Dead || Health <= 0.0f;
	}

	/**
	 * 应用伤害
	 * @param Damage 伤害值
	 * @return 是否死亡
	 */
	bool ApplyDamage(float Damage)
	{
		if (!IsAlive())
		{
			return true;
		}

		Health -= Damage;
		
		if (Health <= 0.0f)
		{
			Health = 0.0f;
			State = EEnemyState::Dying;
			return true;
		}

		return false;
	}

	/**
	 * 获取生命值百分比
	 * @return 生命值百分比（0.0 - 1.0）
	 */
	float GetHealthPercentage() const
	{
		if (MaxHealth <= 0.0f)
		{
			return 0.0f;
		}
		return FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f);
	}

	/**
	 * 设置状态
	 * @param NewState 新状态
	 */
	void SetState(EEnemyState NewState)
	{
		State = NewState;
	}
};
