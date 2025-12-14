// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/SpecialEffectBlueprintLibrary.h"

FGravityWellParams USpecialEffectBlueprintLibrary::MakeGravityWellParams(
	FVector Position,
	float GravityStrength,
	float EffectRadius,
	float Duration,
	bool bDestroyOnReach)
{
	FGravityWellParams Params;
	Params.Position = Position;
	Params.GravityStrength = GravityStrength;
	Params.EffectRadius = EffectRadius;
	Params.Duration = Duration;
	Params.bDestroyOnReach = bDestroyOnReach;
	return Params;
}

FWormholeParams USpecialEffectBlueprintLibrary::MakeWormholeParams(
	FVector EntrancePosition,
	FVector ExitPosition,
	float EntranceRadius,
	float ExitSpeedMultiplier,
	float Duration)
{
	FWormholeParams Params;
	Params.EntrancePosition = EntrancePosition;
	Params.ExitPosition = ExitPosition;
	Params.EntranceRadius = EntranceRadius;
	Params.ExitSpeedMultiplier = ExitSpeedMultiplier;
	Params.Duration = Duration;
	return Params;
}

FSplitParams USpecialEffectBlueprintLibrary::MakeSplitParams(
	int32 SplitCount,
	float SplitAngle,
	float SpeedMultiplier,
	float PotencyMultiplier,
	float RadiusMultiplier)
{
	FSplitParams Params;
	Params.SplitCount = SplitCount;
	Params.SplitAngle = SplitAngle;
	Params.SpeedMultiplier = SpeedMultiplier;
	Params.PotencyMultiplier = PotencyMultiplier;
	Params.RadiusMultiplier = RadiusMultiplier;
	return Params;
}

FEchoSpeedModifierParams USpecialEffectBlueprintLibrary::MakeSpeedModifierParams(
	float SpeedMultiplier,
	bool bApplyImmediately,
	float Duration)
{
	FEchoSpeedModifierParams Params;
	Params.SpeedMultiplier = SpeedMultiplier;
	Params.bApplyImmediately = bApplyImmediately;
	Params.Duration = Duration;
	return Params;
}

FChainTriggerParams USpecialEffectBlueprintLibrary::MakeChainTriggerParams(
	int32 SecondaryCount,
	float AngleRange,
	float SecondarySpeed,
	float PotencyMultiplier,
	float SecondaryRadius)
{
	FChainTriggerParams Params;
	Params.SecondaryCount = SecondaryCount;
	Params.AngleRange = AngleRange;
	Params.SecondarySpeed = SecondarySpeed;
	Params.PotencyMultiplier = PotencyMultiplier;
	Params.SecondaryRadius = SecondaryRadius;
	return Params;
}

FGravityWellParams USpecialEffectBlueprintLibrary::MakeWeakGravityWell(FVector Position)
{
	FGravityWellParams Params;
	Params.Position = Position;
	Params.GravityStrength = 500.0f;
	Params.EffectRadius = 300.0f;
	Params.Duration = 3.0f;
	Params.bDestroyOnReach = false;
	return Params;
}

FGravityWellParams USpecialEffectBlueprintLibrary::MakeStrongGravityWell(FVector Position)
{
	FGravityWellParams Params;
	Params.Position = Position;
	Params.GravityStrength = 2000.0f;
	Params.EffectRadius = 600.0f;
	Params.Duration = 10.0f;
	Params.bDestroyOnReach = true;
	return Params;
}

FSplitParams USpecialEffectBlueprintLibrary::MakeBinarySplit()
{
	FSplitParams Params;
	Params.SplitCount = 2;
	Params.SplitAngle = 30.0f;
	Params.SpeedMultiplier = 0.8f;
	Params.PotencyMultiplier = 0.5f;
	Params.RadiusMultiplier = 0.7f;
	return Params;
}

FSplitParams USpecialEffectBlueprintLibrary::MakeTripleSplit()
{
	FSplitParams Params;
	Params.SplitCount = 3;
	Params.SplitAngle = 45.0f;
	Params.SpeedMultiplier = 0.75f;
	Params.PotencyMultiplier = 0.4f;
	Params.RadiusMultiplier = 0.6f;
	return Params;
}

FEchoSpeedModifierParams USpecialEffectBlueprintLibrary::MakeSpeedBoost()
{
	FEchoSpeedModifierParams Params;
	Params.SpeedMultiplier = 2.0f;
	Params.bApplyImmediately = true;
	Params.Duration = 0.0f;
	return Params;
}

FEchoSpeedModifierParams USpecialEffectBlueprintLibrary::MakeSpeedSlow()
{
	FEchoSpeedModifierParams Params;
	Params.SpeedMultiplier = 0.5f;
	Params.bApplyImmediately = true;
	Params.Duration = 0.0f;
	return Params;
}

FString USpecialEffectBlueprintLibrary::GetEffectTypeName(EEchoSpecialEffectType EffectType)
{
	switch (EffectType)
	{
	case EEchoSpecialEffectType::None:
		return TEXT("None");
	case EEchoSpecialEffectType::GravityWell:
		return TEXT("Gravity Well");
	case EEchoSpecialEffectType::Wormhole:
		return TEXT("Wormhole");
	case EEchoSpecialEffectType::Split:
		return TEXT("Split");
	case EEchoSpecialEffectType::SpeedBoost:
		return TEXT("Speed Boost");
	case EEchoSpecialEffectType::SpeedSlow:
		return TEXT("Speed Slow");
	case EEchoSpecialEffectType::ChainTrigger:
		return TEXT("Chain Trigger");
	default:
		return TEXT("Unknown");
	}
}

bool USpecialEffectBlueprintLibrary::IsContinuousEffect(EEchoSpecialEffectType EffectType)
{
	switch (EffectType)
	{
	case EEchoSpecialEffectType::GravityWell:
	case EEchoSpecialEffectType::Wormhole:
		return true;
		
	case EEchoSpecialEffectType::Split:
	case EEchoSpecialEffectType::SpeedBoost:
	case EEchoSpecialEffectType::SpeedSlow:
	case EEchoSpecialEffectType::ChainTrigger:
		return false;
		
	default:
		return false;
	}
}
