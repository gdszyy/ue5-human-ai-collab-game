// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/SpecialEffectsManager.h"
#include "Kismet/KismetMathLibrary.h"

// ========== 引力奇点 ==========

FGuid USpecialEffectsManager::CreateGravitySingularity(const FGravityWellParams& Params)
{
	FGravityWellParams NewParams = Params;
	NewParams.CreationTime = CurrentGameTime;
	
	FGuid SingularityID = NewParams.ID;
	GravitySingularities.Add(SingularityID, NewParams);
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectsManager] Gravity singularity created: ID=%s, Strength=%.2f, Radius=%.2f"),
		*SingularityID.ToString(), Params.GravityStrength, Params.EffectRadius);
	
	return SingularityID;
}

bool USpecialEffectsManager::RemoveGravitySingularity(const FGuid& SingularityID)
{
	if (GravitySingularities.Remove(SingularityID) > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[SpecialEffectsManager] Gravity singularity removed: ID=%s"), 
			*SingularityID.ToString());
		return true;
	}
	
	return false;
}

TArray<FGravityWellParams> USpecialEffectsManager::GetAllGravitySingularities() const
{
	TArray<FGravityWellParams> Result;
	GravitySingularities.GenerateValueArray(Result);
	return Result;
}

// ========== 虫洞传送 ==========

FGuid USpecialEffectsManager::CreateWormhole(const FWormholeParams& Params)
{
	FWormholeParams NewParams = Params;
	NewParams.CreationTime = CurrentGameTime;
	
	FGuid WormholeID = NewParams.ID;
	Wormholes.Add(WormholeID, NewParams);
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectsManager] Wormhole created: ID=%s, Entry=%s, Exit=%s"),
		*WormholeID.ToString(), *Params.EntrancePosition.ToString(), *Params.ExitPosition.ToString());
	
	return WormholeID;
}

bool USpecialEffectsManager::RemoveWormhole(const FGuid& WormholeID)
{
	if (Wormholes.Remove(WormholeID) > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[SpecialEffectsManager] Wormhole removed: ID=%s"), 
			*WormholeID.ToString());
		return true;
	}
	
	return false;
}

TArray<FWormholeParams> USpecialEffectsManager::GetAllWormholes() const
{
	TArray<FWormholeParams> Result;
	Wormholes.GenerateValueArray(Result);
	return Result;
}

// ========== 效果应用 ==========

void USpecialEffectsManager::ApplyEffects(TArray<FMarbleState>& Marbles, float DeltaTime)
{
	// 应用引力场
	for (FMarbleState& Marble : Marbles)
	{
		ApplyGravityFields(Marble, DeltaTime);
	}
	
	// 应用虫洞传送
	for (FMarbleState& Marble : Marbles)
	{
		ApplyWormholes(Marble);
	}
}

int32 USpecialEffectsManager::ApplyMarbleSplit(const FMarbleState& ParentMarble, 
                                                const FSplitParams& Params, 
                                                TArray<FMarbleState>& OutChildMarbles)
{
	OutChildMarbles.Empty();
	
	// 检查最大分裂深度
	if (ParentMarble.Generation >= Params.MaxSplitDepth)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SpecialEffectsManager] Marble split blocked: Max depth reached (Generation=%d)"),
			ParentMarble.Generation);
		return 0;
	}
	
	// 计算分裂方向
	TArray<FVector> Directions;
	CalculateSplitDirections(ParentMarble.Velocity, Params.SplitCount, Params.AngleRange, Directions);
	
	// 计算子代速度大小
	float ChildSpeed = ParentMarble.Velocity.Size() * Params.SpeedMultiplier;
	
	// 生成子代魔力露珠
	for (int32 i = 0; i < Params.SplitCount; ++i)
	{
		FMarbleState ChildMarble;
		ChildMarble.Position = ParentMarble.Position;
		ChildMarble.Velocity = Directions[i] * ChildSpeed;
		ChildMarble.EffectRadius = ParentMarble.EffectRadius;
		ChildMarble.Mass = ParentMarble.Mass;
		ChildMarble.PotencyMultiplier = ParentMarble.PotencyMultiplier * Params.PotencyMultiplier;
		ChildMarble.MaxPotencyMultiplier = ParentMarble.MaxPotencyMultiplier * Params.PotencyMultiplier;
		ChildMarble.Generation = ParentMarble.Generation + 1;
		ChildMarble.PotionType = ParentMarble.PotionType;
		ChildMarble.BaseDamage = ParentMarble.BaseDamage;
		ChildMarble.CreationTime = CurrentGameTime;
		ChildMarble.LastUpdateTime = CurrentGameTime;
		
		OutChildMarbles.Add(ChildMarble);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectsManager] Marble split: Parent=%s, Children=%d, Generation=%d"),
		*ParentMarble.ID.ToString(), Params.SplitCount, ParentMarble.Generation + 1);
	
	return OutChildMarbles.Num();
}

void USpecialEffectsManager::ApplySpeedModifier(FMarbleState& Marble, const FEchoSpeedModifierParams& Params)
{
	// 应用速度倍率
	Marble.Velocity *= Params.SpeedMultiplier;
	
	UE_LOG(LogTemp, Verbose, TEXT("[SpecialEffectsManager] Speed modifier applied: Marble=%s, Multiplier=%.2f"),
		*Marble.ID.ToString(), Params.SpeedMultiplier);
}

int32 USpecialEffectsManager::ApplyChainReaction(const FMarbleState& TriggerMarble, 
                                                  const FChainTriggerParams& Params, 
                                                  TArray<FMarbleState>& OutProjectiles)
{
	OutProjectiles.Empty();
	
	// 检查最大连锁深度
	if (TriggerMarble.Generation >= Params.MaxChainDepth)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SpecialEffectsManager] Chain reaction blocked: Max depth reached (Generation=%d)"),
			TriggerMarble.Generation);
		return 0;
	}
	
	// 计算发射方向
	TArray<FVector> Directions;
	CalculateChainDirections(TriggerMarble.Velocity, Params.SecondaryCount, Params.AngleRange, Directions);
	
	// 生成次级魔药
	for (int32 i = 0; i < Params.SecondaryCount; ++i)
	{
		FMarbleState Projectile;
		Projectile.Position = TriggerMarble.Position;
		Projectile.Velocity = Directions[i] * Params.SecondarySpeed;
		Projectile.EffectRadius = TriggerMarble.EffectRadius * 0.8f;  // 次级魔药稍小
		Projectile.Mass = TriggerMarble.Mass * 0.8f;
		Projectile.PotencyMultiplier = Params.PotencyMultiplier;
		Projectile.MaxPotencyMultiplier = Params.PotencyMultiplier;
		Projectile.Generation = TriggerMarble.Generation + 1;
		Projectile.PotionType = TriggerMarble.PotionType;
		Projectile.BaseDamage = TriggerMarble.BaseDamage * Params.DamageMultiplier;
		Projectile.CreationTime = CurrentGameTime;
		Projectile.LastUpdateTime = CurrentGameTime;
		
		OutProjectiles.Add(Projectile);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectsManager] Chain reaction: Trigger=%s, Projectiles=%d, Generation=%d"),
		*TriggerMarble.ID.ToString(), Params.SecondaryCount, TriggerMarble.Generation + 1);
	
	return OutProjectiles.Num();
}

// ========== 系统管理 ==========

void USpecialEffectsManager::Tick(float DeltaTime)
{
	// 更新游戏时间
	CurrentGameTime += DeltaTime;
	
	// 清理过期的效果
	CleanupExpiredEffects();
}

void USpecialEffectsManager::ClearAllEffects()
{
	GravitySingularities.Empty();
	Wormholes.Empty();
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectsManager] All effects cleared"));
}

void USpecialEffectsManager::GetStatistics(int32& OutGravityCount, int32& OutWormholeCount) const
{
	OutGravityCount = GravitySingularities.Num();
	OutWormholeCount = Wormholes.Num();
}

// ========== 内部辅助函数 ==========

void USpecialEffectsManager::ApplyGravityFields(FMarbleState& Marble, float DeltaTime)
{
	for (const auto& Pair : GravitySingularities)
	{
		const FGravityWellParams& Singularity = Pair.Value;
		
		// 计算距离
		FVector Delta = Singularity.Position - Marble.Position;
		float Distance = Delta.Size();
		
		// 检查是否在影响范围内
		if (Distance > Singularity.EffectRadius || Distance < KINDA_SMALL_NUMBER)
		{
			continue;
		}
		
		// 计算引力加速度（平方反比定律）
		FVector Direction = Delta / Distance;
		float Acceleration = Singularity.GravityStrength * (1.0f - Distance / Singularity.EffectRadius);
		
		// 应用引力
		Marble.Velocity += Direction * Acceleration * DeltaTime;
	}
}

void USpecialEffectsManager::ApplyWormholes(FMarbleState& Marble)
{
	for (const auto& Pair : Wormholes)
	{
		const FWormholeParams& Wormhole = Pair.Value;
		
		// 计算距离
		FVector Delta = Wormhole.EntrancePosition - Marble.Position;
		float Distance = Delta.Size();
		
		// 检查是否进入虫洞
		if (Distance <= Wormhole.EntranceRadius)
		{
			// 传送到出口
			Marble.Position = Wormhole.ExitPosition;
			
			// 处理速度
			if (Wormhole.bPreserveVelocity)
			{
				Marble.Velocity *= Wormhole.ExitSpeedMultiplier;
			}
			else
			{
				// 随机方向
				FVector RandomDirection = FVector(
					FMath::FRandRange(-1.0f, 1.0f),
					FMath::FRandRange(-1.0f, 1.0f),
					FMath::FRandRange(-1.0f, 1.0f)
				).GetSafeNormal();
				
				float Speed = Marble.Velocity.Size() * Wormhole.ExitSpeedMultiplier;
				Marble.Velocity = RandomDirection * Speed;
			}
			
			UE_LOG(LogTemp, Verbose, TEXT("[SpecialEffectsManager] Marble teleported: ID=%s, From=%s, To=%s"),
				*Marble.ID.ToString(), *Wormhole.EntrancePosition.ToString(), *Wormhole.ExitPosition.ToString());
			
			// 只传送一次，跳出循环
			break;
		}
	}
}

void USpecialEffectsManager::CleanupExpiredEffects()
{
	// 清理过期的引力奇点
	TArray<FGuid> ExpiredSingularities;
	for (const auto& Pair : GravitySingularities)
	{
		if (Pair.Value.IsExpired(CurrentGameTime))
		{
			ExpiredSingularities.Add(Pair.Key);
		}
	}
	
	for (const FGuid& ID : ExpiredSingularities)
	{
		GravitySingularities.Remove(ID);
	}
	
	// 清理过期的虫洞
	TArray<FGuid> ExpiredWormholes;
	for (const auto& Pair : Wormholes)
	{
		if (Pair.Value.IsExpired(CurrentGameTime))
		{
			ExpiredWormholes.Add(Pair.Key);
		}
	}
	
	for (const FGuid& ID : ExpiredWormholes)
	{
		Wormholes.Remove(ID);
	}
	
	if (ExpiredSingularities.Num() > 0 || ExpiredWormholes.Num() > 0)
	{
		UE_LOG(LogTemp, Verbose, TEXT("[SpecialEffectsManager] Cleaned up expired effects: Singularities=%d, Wormholes=%d"),
			ExpiredSingularities.Num(), ExpiredWormholes.Num());
	}
}

void USpecialEffectsManager::CalculateSplitDirections(const FVector& ParentVelocity, 
                                                       int32 SplitCount, 
                                                       float AngleSpread, 
                                                       TArray<FVector>& OutDirections)
{
	OutDirections.Empty();
	
	// 获取父代方向
	FVector ParentDirection = ParentVelocity.GetSafeNormal();
	
	// 如果只分裂成1个，直接返回父代方向
	if (SplitCount == 1)
	{
		OutDirections.Add(ParentDirection);
		return;
	}
	
	// 计算垂直于速度方向的平面
	FVector Up = FVector::UpVector;
	if (FMath::Abs(FVector::DotProduct(ParentDirection, Up)) > 0.9f)
	{
		Up = FVector::RightVector;
	}
	
	FVector Right = FVector::CrossProduct(ParentDirection, Up).GetSafeNormal();
	Up = FVector::CrossProduct(Right, ParentDirection).GetSafeNormal();
	
	// 计算每个子代的方向
	float AngleStep = AngleSpread / (SplitCount - 1);
	float StartAngle = -AngleSpread / 2.0f;
	
	for (int32 i = 0; i < SplitCount; ++i)
	{
		float Angle = StartAngle + AngleStep * i;
		float AngleRad = FMath::DegreesToRadians(Angle);
		
		// 绕Up轴旋转
		FVector Direction = ParentDirection * FMath::Cos(AngleRad) + Right * FMath::Sin(AngleRad);
		Direction.Normalize();
		
		OutDirections.Add(Direction);
	}
}

void USpecialEffectsManager::CalculateChainDirections(const FVector& TriggerVelocity, 
                                                       int32 ProjectileCount, 
                                                       float AngleSpread, 
                                                       TArray<FVector>& OutDirections)
{
	OutDirections.Empty();
	
	// 获取触发方向
	FVector TriggerDirection = TriggerVelocity.GetSafeNormal();
	
	// 如果只发射1个，直接返回触发方向
	if (ProjectileCount == 1)
	{
		OutDirections.Add(TriggerDirection);
		return;
	}
	
	// 计算垂直于速度方向的平面
	FVector Up = FVector::UpVector;
	if (FMath::Abs(FVector::DotProduct(TriggerDirection, Up)) > 0.9f)
	{
		Up = FVector::RightVector;
	}
	
	FVector Right = FVector::CrossProduct(TriggerDirection, Up).GetSafeNormal();
	Up = FVector::CrossProduct(Right, TriggerDirection).GetSafeNormal();
	
	// 计算每个次级魔药的方向（扇形分布）
	float AngleStep = AngleSpread / (ProjectileCount - 1);
	float StartAngle = -AngleSpread / 2.0f;
	
	for (int32 i = 0; i < ProjectileCount; ++i)
	{
		float Angle = StartAngle + AngleStep * i;
		float AngleRad = FMath::DegreesToRadians(Angle);
		
		// 绕Up轴旋转
		FVector Direction = TriggerDirection * FMath::Cos(AngleRad) + Right * FMath::Sin(AngleRad);
		Direction.Normalize();
		
		OutDirections.Add(Direction);
	}
}
