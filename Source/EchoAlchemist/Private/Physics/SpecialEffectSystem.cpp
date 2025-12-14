// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/SpecialEffectSystem.h"

void USpecialEffectSystem::Initialize()
{
	Effects.Empty();
	CurrentGameTime = 0.0f;
	bIsInitialized = true;
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectSystem] Initialized"));
}

void USpecialEffectSystem::Cleanup()
{
	Effects.Empty();
	CurrentGameTime = 0.0f;
	bIsInitialized = false;
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectSystem] Cleaned up"));
}

FGuid USpecialEffectSystem::CreateGravityWell(const FGravityWellParams& Params)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[SpecialEffectSystem] Cannot create gravity well: System not initialized"));
		return FGuid();
	}
	
	FSpecialEffectData Effect;
	Effect.EffectID = FGuid::NewGuid();
	Effect.EffectType = EEchoSpecialEffectType::GravityWell;
	Effect.CreationTime = CurrentGameTime;
	Effect.bIsActive = true;
	Effect.GravityWellParams = Params;
	
	Effects.Add(Effect.EffectID, Effect);
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectSystem] Created gravity well: ID=%s, Position=%s, Strength=%.2f, Radius=%.2f"),
		*Effect.EffectID.ToString(),
		*Params.Position.ToString(),
		Params.GravityStrength,
		Params.EffectRadius);
	
	return Effect.EffectID;
}

FGuid USpecialEffectSystem::CreateWormhole(const FWormholeParams& Params)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[SpecialEffectSystem] Cannot create wormhole: System not initialized"));
		return FGuid();
	}
	
	FSpecialEffectData Effect;
	Effect.EffectID = FGuid::NewGuid();
	Effect.EffectType = EEchoSpecialEffectType::Wormhole;
	Effect.CreationTime = CurrentGameTime;
	Effect.bIsActive = true;
	Effect.WormholeParams = Params;
	
	Effects.Add(Effect.EffectID, Effect);
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectSystem] Created wormhole: ID=%s, Entrance=%s, Exit=%s"),
		*Effect.EffectID.ToString(),
		*Params.EntrancePosition.ToString(),
		*Params.ExitPosition.ToString());
	
	return Effect.EffectID;
}

int32 USpecialEffectSystem::ApplySplitEffect(const FMarbleState& MarbleState, const FSplitParams& Params, TArray<FMarbleState>& OutNewMarbles)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[SpecialEffectSystem] Cannot apply split effect: System not initialized"));
		return 0;
	}
	
	OutNewMarbles.Empty();
	
	// 计算分裂方向
	FVector BaseDirection = MarbleState.Velocity.GetSafeNormal();
	if (BaseDirection.IsNearlyZero())
	{
		BaseDirection = FVector(1, 0, 0);
	}
	
	// 生成子弹珠
	for (int32 i = 0; i < Params.SplitCount; ++i)
	{
		FMarbleState NewMarble;
		NewMarble.ID = FGuid::NewGuid();
		NewMarble.Position = MarbleState.Position;
		NewMarble.Radius = MarbleState.Radius * Params.RadiusMultiplier;
		NewMarble.Mass = MarbleState.Mass * FMath::Pow(Params.RadiusMultiplier, 3.0f);  // 质量与体积成正比
		NewMarble.Potency = MarbleState.Potency * Params.PotencyMultiplier;
		NewMarble.Generation = MarbleState.Generation + 1;
		NewMarble.bIsActive = true;
		
		// 计算分裂角度
		float AngleStep = Params.SplitAngle / FMath::Max(1, Params.SplitCount - 1);
		float CurrentAngle = -Params.SplitAngle / 2.0f + AngleStep * i;
		
		// 旋转速度向量
		FVector RotatedDirection = BaseDirection.RotateAngleAxis(CurrentAngle, FVector(0, 0, 1));
		NewMarble.Velocity = RotatedDirection * MarbleState.Velocity.Size() * Params.SpeedMultiplier;
		
		OutNewMarbles.Add(NewMarble);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectSystem] Applied split effect: MarbleID=%s, SplitCount=%d"),
		*MarbleState.ID.ToString(),
		Params.SplitCount);
	
	// 触发事件
	OnEffectTriggered.Broadcast(EEchoSpecialEffectType::Split, MarbleState.ID);
	
	return OutNewMarbles.Num();
}

bool USpecialEffectSystem::ApplySpeedModifier(const FMarbleState& MarbleState, const FEchoSpeedModifierParams& Params, FMarbleState& OutModifiedState)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[SpecialEffectSystem] Cannot apply speed modifier: System not initialized"));
		return false;
	}
	
	OutModifiedState = MarbleState;
	
	if (Params.bApplyImmediately)
	{
		OutModifiedState.Velocity *= Params.SpeedMultiplier;
		
		UE_LOG(LogTemp, Verbose, TEXT("[SpecialEffectSystem] Applied speed modifier: MarbleID=%s, Multiplier=%.2f"),
			*MarbleState.ID.ToString(),
			Params.SpeedMultiplier);
		
		// 触发事件
		EEchoSpecialEffectType EffectType = Params.SpeedMultiplier > 1.0f ? EEchoSpecialEffectType::SpeedBoost : EEchoSpecialEffectType::SpeedSlow;
		OnEffectTriggered.Broadcast(EffectType, MarbleState.ID);
		
		return true;
	}
	
	return false;
}

int32 USpecialEffectSystem::ApplyChainTrigger(FVector TriggerPosition, FVector TriggerDirection, const FChainTriggerParams& Params, TArray<FMarbleState>& OutSecondaryMarbles)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[SpecialEffectSystem] Cannot apply chain trigger: System not initialized"));
		return 0;
	}
	
	OutSecondaryMarbles.Empty();
	
	// 归一化触发方向
	FVector BaseDirection = TriggerDirection.GetSafeNormal();
	if (BaseDirection.IsNearlyZero())
	{
		BaseDirection = FVector(1, 0, 0);
	}
	
	// 生成次级弹珠
	for (int32 i = 0; i < Params.SecondaryCount; ++i)
	{
		FMarbleState SecondaryMarble;
		SecondaryMarble.ID = FGuid::NewGuid();
		SecondaryMarble.Position = TriggerPosition;
		SecondaryMarble.Radius = Params.SecondaryRadius;
		SecondaryMarble.Mass = 1.0f;  // 默认质量
		SecondaryMarble.Potency = Params.PotencyMultiplier;
		SecondaryMarble.Generation = 1;  // 次级弹珠为第1代
		SecondaryMarble.bIsActive = true;
		
		// 计算发射角度
		float AngleStep = Params.AngleRange / FMath::Max(1, Params.SecondaryCount - 1);
		float CurrentAngle = -Params.AngleRange / 2.0f + AngleStep * i;
		
		// 旋转方向向量
		FVector RotatedDirection = BaseDirection.RotateAngleAxis(CurrentAngle, FVector(0, 0, 1));
		SecondaryMarble.Velocity = RotatedDirection * Params.SecondarySpeed;
		
		OutSecondaryMarbles.Add(SecondaryMarble);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[SpecialEffectSystem] Applied chain trigger: Position=%s, SecondaryCount=%d"),
		*TriggerPosition.ToString(),
		Params.SecondaryCount);
	
	// 触发事件
	OnEffectTriggered.Broadcast(EEchoSpecialEffectType::ChainTrigger, FGuid());
	
	return OutSecondaryMarbles.Num();
}

bool USpecialEffectSystem::RemoveEffect(const FGuid& EffectID)
{
	if (Effects.Remove(EffectID) > 0)
	{
		UE_LOG(LogTemp, Verbose, TEXT("[SpecialEffectSystem] Effect removed: ID=%s"), *EffectID.ToString());
		return true;
	}
	
	return false;
}

void USpecialEffectSystem::Tick(float DeltaTime, const TArray<FMarbleState>& Marbles, TArray<FMarbleState>& OutModifiedMarbles)
{
	if (!bIsInitialized)
	{
		return;
	}
	
	CurrentGameTime += DeltaTime;
	OutModifiedMarbles.Empty();
	
	// 更新所有效果
	for (const auto& Pair : Effects)
	{
		const FSpecialEffectData& Effect = Pair.Value;
		
		if (!Effect.bIsActive)
		{
			continue;
		}
		
		switch (Effect.EffectType)
		{
		case EEchoSpecialEffectType::GravityWell:
			UpdateGravityWell(Effect, Marbles, OutModifiedMarbles);
			break;
			
		case EEchoSpecialEffectType::Wormhole:
			UpdateWormhole(Effect, Marbles, OutModifiedMarbles);
			break;
			
		default:
			break;
		}
	}
	
	// 移除过期的效果
	RemoveExpiredEffects();
}

TArray<FSpecialEffectData> USpecialEffectSystem::GetAllActiveEffects() const
{
	TArray<FSpecialEffectData> ActiveEffects;
	
	for (const auto& Pair : Effects)
	{
		if (Pair.Value.bIsActive)
		{
			ActiveEffects.Add(Pair.Value);
		}
	}
	
	return ActiveEffects;
}

int32 USpecialEffectSystem::GetEffectCountByType(EEchoSpecialEffectType EffectType) const
{
	int32 Count = 0;
	
	for (const auto& Pair : Effects)
	{
		if (Pair.Value.EffectType == EffectType && Pair.Value.bIsActive)
		{
			++Count;
		}
	}
	
	return Count;
}

void USpecialEffectSystem::UpdateGravityWell(const FSpecialEffectData& Effect, const TArray<FMarbleState>& Marbles, TArray<FMarbleState>& OutModifiedMarbles)
{
	const FGravityWellParams& Params = Effect.GravityWellParams;
	
	for (const FMarbleState& Marble : Marbles)
	{
		// 计算距离
		FVector ToWell = Params.Position - Marble.Position;
		float Distance = ToWell.Size();
		
		// 检查是否在影响范围内
		if (Distance > Params.EffectRadius)
		{
			continue;
		}
		
		// 计算引力加速度（反平方定律）
		float GravityAccel = Params.GravityStrength / FMath::Max(Distance * Distance, 1.0f);
		FVector GravityDirection = ToWell.GetSafeNormal();
		
		// 应用引力
		FMarbleState ModifiedMarble = Marble;
		ModifiedMarble.Velocity += GravityDirection * GravityAccel * 0.016f;  // 假设60FPS
		
		OutModifiedMarbles.Add(ModifiedMarble);
	}
}

void USpecialEffectSystem::UpdateWormhole(const FSpecialEffectData& Effect, const TArray<FMarbleState>& Marbles, TArray<FMarbleState>& OutModifiedMarbles)
{
	const FWormholeParams& Params = Effect.WormholeParams;
	
	for (const FMarbleState& Marble : Marbles)
	{
		// 计算到入口的距离
		float DistanceToEntrance = FVector::Dist(Marble.Position, Params.EntrancePosition);
		
		// 检查是否进入虫洞
		if (DistanceToEntrance <= Params.EntranceRadius)
		{
			// 传送到出口
			FMarbleState ModifiedMarble = Marble;
			ModifiedMarble.Position = Params.ExitPosition;
			ModifiedMarble.Velocity *= Params.ExitSpeedMultiplier;
			
			OutModifiedMarbles.Add(ModifiedMarble);
			
			UE_LOG(LogTemp, Verbose, TEXT("[SpecialEffectSystem] Marble teleported: ID=%s, From=%s, To=%s"),
				*Marble.ID.ToString(),
				*Params.EntrancePosition.ToString(),
				*Params.ExitPosition.ToString());
			
			// 触发事件
			OnEffectTriggered.Broadcast(EEchoSpecialEffectType::Wormhole, Marble.ID);
		}
	}
}

void USpecialEffectSystem::RemoveExpiredEffects()
{
	TArray<FGuid> ExpiredEffects;
	
	for (const auto& Pair : Effects)
	{
		const FSpecialEffectData& Effect = Pair.Value;
		
		// 检查持续时间
		float Duration = 0.0f;
		switch (Effect.EffectType)
		{
		case EEchoSpecialEffectType::GravityWell:
			Duration = Effect.GravityWellParams.Duration;
			break;
			
		case EEchoSpecialEffectType::Wormhole:
			Duration = Effect.WormholeParams.Duration;
			break;
			
		default:
			continue;
		}
		
		// 如果Duration为0，表示永久效果
		if (Duration > 0.0f && (CurrentGameTime - Effect.CreationTime) >= Duration)
		{
			ExpiredEffects.Add(Effect.EffectID);
		}
	}
	
	// 移除过期效果
	for (const FGuid& EffectID : ExpiredEffects)
	{
		RemoveEffect(EffectID);
	}
}
