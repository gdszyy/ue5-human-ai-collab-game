// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/MarblePhysicsSystem.h"
#include "Engine/World.h"

void UMarblePhysicsSystem::InitializeScene(const FPhysicsSceneConfig& Config)
{
	// 清空现有魔力露珠
	ActiveMarbles.Empty();
	
	// 保存场景配置
	SceneConfig = Config;
	
	// 重置游戏时间
	CurrentGameTime = 0.0f;
	
	// 标记为已初始化
	bIsInitialized = true;
	
	UE_LOG(LogTemp, Log, TEXT("[MarblePhysicsSystem] Scene initialized: Type=%s, Gravity=%s"),
		*UEnum::GetValueAsString(Config.SceneType),
		Config.bEnableGravity ? TEXT("Enabled") : TEXT("Disabled"));
}

void UMarblePhysicsSystem::CleanupScene()
{
	// 清空所有魔力露珠
	ActiveMarbles.Empty();
	
	// 重置状态
	bIsInitialized = false;
	CurrentGameTime = 0.0f;
	
	UE_LOG(LogTemp, Log, TEXT("[MarblePhysicsSystem] Scene cleaned up"));
}

FGuid UMarblePhysicsSystem::LaunchMarble(const FMarbleLaunchParams& Params)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[MarblePhysicsSystem] Cannot launch marble: System not initialized"));
		return FGuid();
	}
	
	// 创建新的魔力露珠状态
	FMarbleState NewMarble;
	NewMarble.Position = Params.LaunchPosition;
	NewMarble.Velocity = Params.LaunchDirection.GetSafeNormal() * Params.LaunchSpeed;
	NewMarble.EffectRadius = Params.EffectRadius;
	NewMarble.Mass = Params.Mass;
	NewMarble.PotencyMultiplier = Params.PotencyMultiplier;
	NewMarble.MaxPotencyMultiplier = Params.PotencyMultiplier;
	NewMarble.Generation = Params.Generation;
	NewMarble.CreationTime = CurrentGameTime;
	NewMarble.LastUpdateTime = CurrentGameTime;
	
	// 决定是否使用粒子系统
	NewMarble.bUseParticle = ShouldUseParticle(Params.Generation);
	
	// 添加到活跃列表
	FGuid MarbleID = NewMarble.ID;
	ActiveMarbles.Add(MarbleID, NewMarble);
	
	UE_LOG(LogTemp, Log, TEXT("[MarblePhysicsSystem] Marble launched: ID=%s, Generation=%d, UseParticle=%s"),
		*MarbleID.ToString(),
		Params.Generation,
		NewMarble.bUseParticle ? TEXT("Yes") : TEXT("No"));
	
	return MarbleID;
}

bool UMarblePhysicsSystem::RemoveMarble(const FGuid& MarbleID)
{
	if (ActiveMarbles.Remove(MarbleID) > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[MarblePhysicsSystem] Marble removed: ID=%s"), *MarbleID.ToString());
		return true;
	}
	
	return false;
}

bool UMarblePhysicsSystem::GetMarbleState(const FGuid& MarbleID, FMarbleState& OutState) const
{
	const FMarbleState* Found = ActiveMarbles.Find(MarbleID);
	if (Found)
	{
		OutState = *Found;
		return true;
	}
	
	return false;
}

TArray<FMarbleState> UMarblePhysicsSystem::GetAllMarbles() const
{
	TArray<FMarbleState> Result;
	ActiveMarbles.GenerateValueArray(Result);
	return Result;
}

int32 UMarblePhysicsSystem::GetMarbleCount() const
{
	return ActiveMarbles.Num();
}

bool UMarblePhysicsSystem::AreAllMarblesStopped(float SpeedThreshold) const
{
	for (const auto& Pair : ActiveMarbles)
	{
		const FMarbleState& Marble = Pair.Value;
		if (Marble.Velocity.Size() > SpeedThreshold)
		{
			return false;
		}
	}
	
	return true;
}

void UMarblePhysicsSystem::Tick(float DeltaTime)
{
	if (!bIsInitialized)
	{
		return;
	}
	
	// 更新游戏时间
	CurrentGameTime += DeltaTime;
	
	// 收集需要删除的魔力露珠
	TArray<FGuid> MarblesToRemove;
	
	// 更新所有魔力露珠
	for (auto& Pair : ActiveMarbles)
	{
		FMarbleState& Marble = Pair.Value;
		
		// 更新物理状态
		UpdateMarblePhysics(Marble, DeltaTime);
		
		// 检查是否应该删除
		if (ShouldRemoveMarble(Marble))
		{
			MarblesToRemove.Add(Pair.Key);
		}
	}
	
	// 删除无效的魔力露珠
	for (const FGuid& MarbleID : MarblesToRemove)
	{
		RemoveMarble(MarbleID);
	}
}

void UMarblePhysicsSystem::UpdateMarblePhysics(FMarbleState& Marble, float DeltaTime)
{
	// 应用重力
	if (SceneConfig.bEnableGravity)
	{
		ApplyGravity(Marble, DeltaTime);
	}
	
	// 更新位置
	Marble.Position += Marble.Velocity * DeltaTime;
	
	// 处理边界
	if (SceneConfig.bHasBoundary)
	{
		HandleBoundary(Marble);
	}
	
	// 更新时间戳
	Marble.LastUpdateTime = CurrentGameTime;
}

void UMarblePhysicsSystem::ApplyGravity(FMarbleState& Marble, float DeltaTime)
{
	// 计算重力加速度向量
	FVector GravityAcceleration = SceneConfig.GravityDirection * SceneConfig.GravityStrength;
	
	// 更新速度
	Marble.Velocity += GravityAcceleration * DeltaTime;
}

bool UMarblePhysicsSystem::HandleBoundary(FMarbleState& Marble)
{
	const FBox& Boundary = SceneConfig.BoundaryBox;
	bool bHitBoundary = false;
	
	// 检查X轴边界
	if (Marble.Position.X - Marble.EffectRadius < Boundary.Min.X)
	{
		Marble.Position.X = Boundary.Min.X + Marble.EffectRadius;
		if (SceneConfig.BoundaryBehavior == EBoundaryBehavior::Boundary_Bounce)
		{
			Marble.Velocity.X = FMath::Abs(Marble.Velocity.X);
		}
		bHitBoundary = true;
	}
	else if (Marble.Position.X + Marble.EffectRadius > Boundary.Max.X)
	{
		Marble.Position.X = Boundary.Max.X - Marble.EffectRadius;
		if (SceneConfig.BoundaryBehavior == EBoundaryBehavior::Boundary_Bounce)
		{
			Marble.Velocity.X = -FMath::Abs(Marble.Velocity.X);
		}
		bHitBoundary = true;
	}
	
	// 检查Y轴边界
	if (Marble.Position.Y - Marble.EffectRadius < Boundary.Min.Y)
	{
		Marble.Position.Y = Boundary.Min.Y + Marble.EffectRadius;
		if (SceneConfig.BoundaryBehavior == EBoundaryBehavior::Boundary_Bounce)
		{
			Marble.Velocity.Y = FMath::Abs(Marble.Velocity.Y);
		}
		bHitBoundary = true;
	}
	else if (Marble.Position.Y + Marble.EffectRadius > Boundary.Max.Y)
	{
		Marble.Position.Y = Boundary.Max.Y - Marble.EffectRadius;
		if (SceneConfig.BoundaryBehavior == EBoundaryBehavior::Boundary_Bounce)
		{
			Marble.Velocity.Y = -FMath::Abs(Marble.Velocity.Y);
		}
		bHitBoundary = true;
	}
	
	// 检查Z轴边界
	if (Marble.Position.Z - Marble.EffectRadius < Boundary.Min.Z)
	{
		Marble.Position.Z = Boundary.Min.Z + Marble.EffectRadius;
		if (SceneConfig.BoundaryBehavior == EBoundaryBehavior::Boundary_Bounce)
		{
			Marble.Velocity.Z = FMath::Abs(Marble.Velocity.Z);
		}
		bHitBoundary = true;
	}
	else if (Marble.Position.Z + Marble.EffectRadius > Boundary.Max.Z)
	{
		Marble.Position.Z = Boundary.Max.Z - Marble.EffectRadius;
		if (SceneConfig.BoundaryBehavior == EBoundaryBehavior::Boundary_Bounce)
		{
			Marble.Velocity.Z = -FMath::Abs(Marble.Velocity.Z);
		}
		bHitBoundary = true;
	}
	
	// 如果碰到边界且行为是删除，标记为无效
	if (bHitBoundary && SceneConfig.BoundaryBehavior == EBoundaryBehavior::Boundary_Delete)
	{
		Marble.PotencyMultiplier = 0.0f;  // 标记为无效
		return false;
	}
	
	return true;
}

bool UMarblePhysicsSystem::ShouldRemoveMarble(const FMarbleState& Marble) const
{
	// 检查药效强度（仅战斗场景）
	if (SceneConfig.bUsePotencySystem && Marble.PotencyMultiplier <= 0.0f)
	{
		return true;
	}
	
	return false;
}

bool UMarblePhysicsSystem::ShouldUseParticle(int32 Generation) const
{
	// 如果未启用粒子优化，始终使用Actor
	if (!SceneConfig.bEnableParticleOptimization)
	{
		return false;
	}
	
	// 第0-1代使用Actor，第2代及以上使用粒子
	return Generation >= 2;
}

void UMarblePhysicsSystem::InitializeHybridPhysics(UWorld* World, int32 PreAllocateActorCount)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[MarblePhysicsSystem] Cannot initialize hybrid physics: Scene not initialized"));
		return;
	}

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[MarblePhysicsSystem] Cannot initialize hybrid physics: World is null"));
		return;
	}

	// 创建Actor对象池
	ActorPool = NewObject<UMarbleActorPool>(this);
	ActorPool->Initialize(World, PreAllocateActorCount);

	// TODO: 创建Niagara粒子系统组件（需要在蓝图中配置Niagara资源）
	// ParticleSystem = NewObject<UNiagaraComponent>(this);

	UE_LOG(LogTemp, Log, TEXT("[MarblePhysicsSystem] Hybrid physics initialized: PreAllocated=%d"), 
		PreAllocateActorCount);
}

void UMarblePhysicsSystem::GetActorPoolStatistics(int32& OutTotalCount, int32& OutAvailableCount, int32& OutInUseCount) const
{
	if (ActorPool)
	{
		ActorPool->GetStatistics(OutTotalCount, OutAvailableCount, OutInUseCount);
	}
	else
	{
		OutTotalCount = 0;
		OutAvailableCount = 0;
		OutInUseCount = 0;
	}
}
