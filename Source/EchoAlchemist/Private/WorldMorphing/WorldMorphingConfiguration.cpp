// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldMorphing/WorldMorphingConfiguration.h"
#include "WorldMorphing/WorldMorphingSubsystem.h"
#include "Engine/GameInstance.h"

static UWorldMorphingSubsystem* GetSubsystem(UObject* WorldContextObject)
{
	UWorld* World = nullptr;
	
	// 方法1: 尝试从WorldContextObject获取World
	if (WorldContextObject)
	{
		World = WorldContextObject->GetWorld();
	}
	
	// 方法2: 如果WorldContextObject为nullptr或无法获取World，尝试使用GEngine
	if (!World && GEngine)
	{
		// 在PIE模式下，尝试获取PIE世界
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
			{
				World = Context.World();
				if (World)
				{
					break;
				}
			}
		}
	}
	
	if (!World)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<UWorldMorphingSubsystem>();
}

FSimulationParams UWorldMorphingConfiguration::MakeDefault()
{
	return FSimulationParams();
}

FSimulationParams UWorldMorphingConfiguration::MakeCustom(float ExpansionThreshold,
                                                           float ThunderstormThreshold,
                                                           float AlphaEnergyDemand)
{
	FSimulationParams Params;
	Params.ExpansionThreshold = ExpansionThreshold;
	Params.ThunderstormThreshold = ThunderstormThreshold;
	Params.AlphaEnergyDemand = AlphaEnergyDemand;
	return Params;
}

FSimulationParams UWorldMorphingConfiguration::GetCurrent(UObject* WorldContextObject)
{
	UWorldMorphingSubsystem* Subsystem = GetSubsystem(WorldContextObject);
	if (!Subsystem)
	{
		return FSimulationParams();
	}

	return Subsystem->GetSimulationParams();
}

void UWorldMorphingConfiguration::Apply(UObject* WorldContextObject, const FSimulationParams& Params)
{
	UWorldMorphingSubsystem* Subsystem = GetSubsystem(WorldContextObject);
	if (!Subsystem)
	{
		return;
	}

	Subsystem->SetSimulationParams(Params);
}

void UWorldMorphingConfiguration::ResetToDefault(UObject* WorldContextObject)
{
	UWorldMorphingSubsystem* Subsystem = GetSubsystem(WorldContextObject);
	if (!Subsystem)
	{
		return;
	}

	Subsystem->SetSimulationParams(FSimulationParams());
}

FSimulationParams UWorldMorphingConfiguration::MakePreset(ESimulationPreset Preset)
{
	FSimulationParams Params;

	switch (Preset)
	{
	case ESimulationPreset::Default:
		// 使用默认值
		break;

	case ESimulationPreset::FastGrowth:
		// 快速生长：低扩张阈值，高能量需求
		Params.ExpansionThreshold = 80.0f;
		Params.AlphaEnergyDemand = 1.0f;
		Params.BetaEnergyDemand = 1.5f;
		Params.MantleEnergyLevel = 120.0f;
		Params.EdgeGenerationEnergy = 6.0f;
		break;

	case ESimulationPreset::SlowEvolution:
		// 缓慢演化：高扩张阈值，低能量需求
		Params.ExpansionThreshold = 180.0f;
		Params.AlphaEnergyDemand = 2.5f;
		Params.BetaEnergyDemand = 3.0f;
		Params.MantleEnergyLevel = 80.0f;
		Params.EdgeGenerationEnergy = 2.0f;
		Params.MantleTimeScale = 0.001f;
		break;

	case ESimulationPreset::HighEnergy:
		// 高能量：高雷暴阈值，高地幔能量
		Params.MantleEnergyLevel = 150.0f;
		Params.ThunderstormThreshold = 10.0f;
		Params.ThunderstormEnergy = 20.0f;
		Params.EdgeGenerationEnergy = 8.0f;
		Params.MaxCrystalEnergy = 120.0f;
		break;

	case ESimulationPreset::Stable:
		// 稳定：平衡的参数
		Params.ExpansionThreshold = 150.0f;
		Params.ShrinkThreshold = 3.0f;
		Params.AlphaEnergyDemand = 2.0f;
		Params.BetaEnergyDemand = 2.5f;
		Params.ThunderstormThreshold = 25.0f;
		Params.DiffusionRate = 0.08f;
		break;

	default:
		break;
	}

	return Params;
}
