// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/CombatPhysicsIntegrator.h"
#include "Physics/MarbleState.h"
#include "Combat/EnemyData.h"
#include "Physics/CollisionShape.h"

UCombatPhysicsIntegrator::UCombatPhysicsIntegrator()
{
	CombatManager = nullptr;
	EnemyManager = nullptr;
	PhysicsSystem = nullptr;
	CollisionManager = nullptr;
}

void UCombatPhysicsIntegrator::Initialize(
	UCombatManager* InCombatManager,
	UEnemyManager* InEnemyManager,
	UMarblePhysicsSystem* InPhysicsSystem,
	UCollisionManager* InCollisionManager
)
{
	CombatManager = InCombatManager;
	EnemyManager = InEnemyManager;
	PhysicsSystem = InPhysicsSystem;
	CollisionManager = InCollisionManager;
	
	// 清空映射
	MarbleToCollisionBodyMap.Empty();
	EnemyToCollisionBodyMap.Empty();
	CollisionBodyToMarbleMap.Empty();
	CollisionBodyToEnemyMap.Empty();
	
	UE_LOG(LogTemp, Log, TEXT("CombatPhysicsIntegrator: Initialized"));
}

FGuid UCombatPhysicsIntegrator::LaunchMarble(const FMarbleLaunchParams& Params)
{
	if (!PhysicsSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("CombatPhysicsIntegrator: Physics system not set"));
		return FGuid();
	}
	
	// 发射魔药
	FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);
	
	// 获取魔药状态
	FMarbleState MarbleState;
	if (PhysicsSystem->GetMarbleState(MarbleID, MarbleState))
	{
		// 注册碰撞体
		RegisterMarbleCollisionBody(MarbleID, MarbleState.Position, MarbleState.Radius);
		
		UE_LOG(LogTemp, Log, TEXT("CombatPhysicsIntegrator: Launched marble %s at (%.1f, %.1f, %.1f)"),
			*MarbleID.ToString(), MarbleState.Position.X, MarbleState.Position.Y, MarbleState.Position.Z);
	}
	
	return MarbleID;
}

bool UCombatPhysicsIntegrator::RemoveMarble(FGuid MarbleID)
{
	if (!PhysicsSystem)
	{
		return false;
	}
	
	// 移除碰撞体
	RemoveMarbleCollisionBody(MarbleID);
	
	// 移除魔药
	return PhysicsSystem->RemoveMarble(MarbleID);
}

TArray<FMarbleState> UCombatPhysicsIntegrator::GetAllMarbles() const
{
	if (!PhysicsSystem)
	{
		return TArray<FMarbleState>();
	}
	
	return PhysicsSystem->GetAllMarbles();
}

int32 UCombatPhysicsIntegrator::GetMarbleCount() const
{
	if (!PhysicsSystem)
	{
		return 0;
	}
	
	return PhysicsSystem->GetMarbleCount();
}

void UCombatPhysicsIntegrator::Tick(float DeltaTime)
{
	// 更新物理系统
	UpdatePhysics(DeltaTime);
	
	// 更新碰撞体
	UpdateCollisionBodies();
	
	// 检测碰撞
	DetectCollisions();
}

void UCombatPhysicsIntegrator::HandleCollision(const FCollisionEvent& CollisionEvent)
{
	// 检查是否是魔药与敌人的碰撞
	FGuid* MarbleID1 = CollisionBodyToMarbleMap.Find(CollisionEvent.BodyA);
	FGuid* EnemyID1 = CollisionBodyToEnemyMap.Find(CollisionEvent.BodyB);
	
	if (MarbleID1 && EnemyID1)
	{
		// BodyA是魔药，BodyB是敌人
		HandleMarbleEnemyCollision(*MarbleID1, *EnemyID1);
		return;
	}
	
	// 检查反向碰撞
	FGuid* MarbleID2 = CollisionBodyToMarbleMap.Find(CollisionEvent.BodyB);
	FGuid* EnemyID2 = CollisionBodyToEnemyMap.Find(CollisionEvent.BodyA);
	
	if (MarbleID2 && EnemyID2)
	{
		// BodyB是魔药，BodyA是敌人
		HandleMarbleEnemyCollision(*MarbleID2, *EnemyID2);
		return;
	}
}

bool UCombatPhysicsIntegrator::AreAllMarblesStopped(float SpeedThreshold) const
{
	if (!PhysicsSystem)
	{
		return true;
	}
	
	return PhysicsSystem->AreAllMarblesStopped(SpeedThreshold);
}

void UCombatPhysicsIntegrator::UpdatePhysics(float DeltaTime)
{
	if (!PhysicsSystem)
	{
		return;
	}
	
	// 更新物理系统
	PhysicsSystem->Tick(DeltaTime);
}

void UCombatPhysicsIntegrator::UpdateCollisionBodies()
{
	if (!PhysicsSystem || !CollisionManager)
	{
		return;
	}
	
	// 更新魔药碰撞体
	TArray<FMarbleState> Marbles = PhysicsSystem->GetAllMarbles();
	for (const FMarbleState& Marble : Marbles)
	{
		UpdateMarbleCollisionBody(Marble.ID, Marble.Position);
	}
	
	// 更新敌人碰撞体
	if (EnemyManager)
	{
		TArray<FEnemyData> Enemies = EnemyManager->GetAliveEnemies();
		for (const FEnemyData& Enemy : Enemies)
		{
			// 如果敌人碰撞体不存在，注册它
			if (!EnemyToCollisionBodyMap.Contains(Enemy.ID))
			{
				RegisterEnemyCollisionBody(Enemy.ID, Enemy.Position, 20.0f); // 默认半径20cm
			}
			else
			{
				UpdateEnemyCollisionBody(Enemy.ID, Enemy.Position);
			}
		}
	}
	
	// 更新空间网格
	CollisionManager->UpdateSpatialGrid();
}

void UCombatPhysicsIntegrator::DetectCollisions()
{
	if (!CollisionManager)
	{
		return;
	}
	
	// 检测碰撞
	TArray<FCollisionEvent> Collisions = CollisionManager->DetectCollisions();
	
	// 处理每个碰撞事件
	for (const FCollisionEvent& Collision : Collisions)
	{
		HandleCollision(Collision);
	}
}

void UCombatPhysicsIntegrator::HandleMarbleEnemyCollision(FGuid MarbleID, FGuid EnemyID)
{
	if (!PhysicsSystem || !EnemyManager)
	{
		return;
	}
	
	// 获取魔药状态
	FMarbleState MarbleState;
	if (!PhysicsSystem->GetMarbleState(MarbleID, MarbleState))
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatPhysicsIntegrator: Marble not found: %s"), *MarbleID.ToString());
		return;
	}
	
	// 计算伤害
	FDamageInfo DamageInfo = UDamageCalculator::CalculateDamage(MarbleState, EnemyID);
	
	// 应用伤害到敌人
	bool bDied = EnemyManager->ApplyDamageToEnemy(EnemyID, DamageInfo.FinalDamage);
	
	UE_LOG(LogTemp, Log, TEXT("CombatPhysicsIntegrator: Marble %s hit enemy %s for %.1f damage. Enemy %s"),
		*MarbleID.ToString(), *EnemyID.ToString(), DamageInfo.FinalDamage, bDied ? TEXT("died") : TEXT("survived"));
	
	// 更新魔药状态（增加撞击次数）
	MarbleState.HitCount++;
	
	// 如果敌人死亡，移除敌人碰撞体
	if (bDied)
	{
		RemoveEnemyCollisionBody(EnemyID);
		
		// 增加战斗管理器的击杀数
		if (CombatManager)
		{
			CombatManager->IncrementKillCount();
		}
	}
	
	// 发布碰撞事件
	if (CombatManager)
	{
		FCombatEvent Event;
		Event.EventType = ECombatEventType::MarbleHitEnemy;
		Event.Timestamp = FPlatformTime::Seconds();
		Event.EntityID = MarbleID;
		Event.ExtraData.Add(TEXT("EnemyID"), *reinterpret_cast<const float*>(&EnemyID));
		Event.ExtraData.Add(TEXT("Damage"), DamageInfo.FinalDamage);
		CombatManager->BroadcastEvent(Event);
	}
}

void UCombatPhysicsIntegrator::RegisterMarbleCollisionBody(FGuid MarbleID, FVector Position, float Radius)
{
	if (!CollisionManager)
	{
		return;
	}
	
	// 创建碰撞体
	FCollisionBody Body;
	Body.ID = FGuid::NewGuid();
	Body.Position = Position;
	Body.ShapeType = EEchoCollisionShapeType::Circle;
	Body.Radius = Radius;
	Body.bIsStatic = false;
	
	// 注册碰撞体
	CollisionManager->RegisterBody(Body);
	
	// 建立映射
	MarbleToCollisionBodyMap.Add(MarbleID, Body.ID);
	CollisionBodyToMarbleMap.Add(Body.ID, MarbleID);
	
	UE_LOG(LogTemp, Verbose, TEXT("CombatPhysicsIntegrator: Registered marble collision body %s for marble %s"),
		*Body.ID.ToString(), *MarbleID.ToString());
}

void UCombatPhysicsIntegrator::RegisterEnemyCollisionBody(FGuid EnemyID, FVector Position, float Radius)
{
	if (!CollisionManager)
	{
		return;
	}
	
	// 创建碰撞体
	FCollisionBody Body;
	Body.ID = FGuid::NewGuid();
	Body.Position = Position;
	Body.ShapeType = EEchoCollisionShapeType::Circle;
	Body.Radius = Radius;
	Body.bIsStatic = false;
	
	// 注册碰撞体
	CollisionManager->RegisterBody(Body);
	
	// 建立映射
	EnemyToCollisionBodyMap.Add(EnemyID, Body.ID);
	CollisionBodyToEnemyMap.Add(Body.ID, EnemyID);
	
	UE_LOG(LogTemp, Verbose, TEXT("CombatPhysicsIntegrator: Registered enemy collision body %s for enemy %s"),
		*Body.ID.ToString(), *EnemyID.ToString());
}

void UCombatPhysicsIntegrator::UpdateMarbleCollisionBody(FGuid MarbleID, FVector Position)
{
	if (!CollisionManager)
	{
		return;
	}
	
	// 查找碰撞体ID
	FGuid* CollisionBodyID = MarbleToCollisionBodyMap.Find(MarbleID);
	if (!CollisionBodyID)
	{
		return;
	}
	
	// 更新碰撞体位置
	CollisionManager->UpdateBodyPosition(*CollisionBodyID, Position);
}

void UCombatPhysicsIntegrator::UpdateEnemyCollisionBody(FGuid EnemyID, FVector Position)
{
	if (!CollisionManager)
	{
		return;
	}
	
	// 查找碰撞体ID
	FGuid* CollisionBodyID = EnemyToCollisionBodyMap.Find(EnemyID);
	if (!CollisionBodyID)
	{
		return;
	}
	
	// 更新碰撞体位置
	CollisionManager->UpdateBodyPosition(*CollisionBodyID, Position);
}

void UCombatPhysicsIntegrator::RemoveMarbleCollisionBody(FGuid MarbleID)
{
	if (!CollisionManager)
	{
		return;
	}
	
	// 查找碰撞体ID
	FGuid* CollisionBodyID = MarbleToCollisionBodyMap.Find(MarbleID);
	if (!CollisionBodyID)
	{
		return;
	}
	
	// 移除碰撞体
	CollisionManager->UnregisterBody(*CollisionBodyID);
	
	// 移除映射
	CollisionBodyToMarbleMap.Remove(*CollisionBodyID);
	MarbleToCollisionBodyMap.Remove(MarbleID);
	
	UE_LOG(LogTemp, Verbose, TEXT("CombatPhysicsIntegrator: Removed marble collision body for marble %s"),
		*MarbleID.ToString());
}

void UCombatPhysicsIntegrator::RemoveEnemyCollisionBody(FGuid EnemyID)
{
	if (!CollisionManager)
	{
		return;
	}
	
	// 查找碰撞体ID
	FGuid* CollisionBodyID = EnemyToCollisionBodyMap.Find(EnemyID);
	if (!CollisionBodyID)
	{
		return;
	}
	
	// 移除碰撞体
	CollisionManager->UnregisterBody(*CollisionBodyID);
	
	// 移除映射
	CollisionBodyToEnemyMap.Remove(*CollisionBodyID);
	EnemyToCollisionBodyMap.Remove(EnemyID);
	
	UE_LOG(LogTemp, Verbose, TEXT("CombatPhysicsIntegrator: Removed enemy collision body for enemy %s"),
		*EnemyID.ToString());
}
