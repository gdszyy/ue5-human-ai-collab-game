// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldMorphing/WorldMorphingVisualization.h"
#include "WorldMorphing/WorldMorphingSubsystem.h"
#include "Engine/GameInstance.h"

static UWorldMorphingSubsystem* GetWorldMorphingSubsystemForVisualization(UObject* WorldContextObject)
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

FCellState UWorldMorphingVisualization::GetCellState(UObject* WorldContextObject, int32 X, int32 Y)
{
	UWorldMorphingSubsystem* Subsystem = GetWorldMorphingSubsystemForVisualization(WorldContextObject);
	if (!Subsystem)
	{
		return FCellState();
	}

	return Subsystem->GetCellAt(X, Y);
}

TArray<FCellState> UWorldMorphingVisualization::GetRegionStates(UObject* WorldContextObject, 
                                                                 int32 StartX, int32 StartY, 
                                                                 int32 Width, int32 Height)
{
	TArray<FCellState> States;

	UWorldMorphingSubsystem* Subsystem = GetWorldMorphingSubsystemForVisualization(WorldContextObject);
	if (!Subsystem)
	{
		return States;
	}

	// 预分配数组
	States.Reserve(Width * Height);

	// 批量获取单元格状态
	for (int32 Y = StartY; Y < StartY + Height; ++Y)
	{
		for (int32 X = StartX; X < StartX + Width; ++X)
		{
			States.Add(Subsystem->GetCellAt(X, Y));
		}
	}

	return States;
}

FWorldStatistics UWorldMorphingVisualization::GetStatistics(UObject* WorldContextObject)
{
	FWorldStatistics Stats;

	UWorldMorphingSubsystem* Subsystem = GetWorldMorphingSubsystemForVisualization(WorldContextObject);
	if (!Subsystem)
	{
		return Stats;
	}

	// 获取网格尺寸
	int32 GridWidth, GridHeight;
	Subsystem->GetGridSize(GridWidth, GridHeight);
	Stats.TotalCells = GridWidth * GridHeight;

	// 遍历所有单元格统计
	float TotalMantleEnergy = 0.0f;
	float TotalTemperature = 0.0f;
	int32 ValidCells = 0;

	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			FCellState Cell = Subsystem->GetCellAt(X, Y);

			if (Cell.bExists)
			{
				Stats.TerrainCells++;
				TotalMantleEnergy += Cell.MantleEnergy;
				TotalTemperature += Cell.Temperature;
				ValidCells++;
			}

			// 统计晶石类型
			switch (Cell.CrystalType)
			{
			case ECrystalType::Alpha:
				Stats.AlphaCrystals++;
				break;
			case ECrystalType::Beta:
				Stats.BetaCrystals++;
				break;
			case ECrystalType::Human:
				Stats.HumanSettlements++;
				break;
			default:
				break;
			}

			// 统计雷暴
			if (Cell.bHasThunderstorm)
			{
				Stats.ThunderstormCells++;
			}
		}
	}

	// 计算平均值
	if (ValidCells > 0)
	{
		Stats.AverageMantleEnergy = TotalMantleEnergy / ValidCells;
		Stats.AverageTemperature = TotalTemperature / ValidCells;
	}

	return Stats;
}

TArray<float> UWorldMorphingVisualization::GetHeatmapData(UObject* WorldContextObject, 
                                                           EHeatmapDataType DataType)
{
	TArray<float> HeatmapData;

	UWorldMorphingSubsystem* Subsystem = GetWorldMorphingSubsystemForVisualization(WorldContextObject);
	if (!Subsystem)
	{
		return HeatmapData;
	}

	// 获取网格尺寸
	int32 GridWidth, GridHeight;
	Subsystem->GetGridSize(GridWidth, GridHeight);

	// 预分配数组
	HeatmapData.Reserve(GridWidth * GridHeight);

	// 根据数据类型提取不同的值
	switch (DataType)
	{
	case EHeatmapDataType::MantleEnergy:
		{
			// 地幔能量热力图（归一化到0-1）
			const float MaxEnergy = 100.0f; // 假设最大能量为100
			for (int32 Y = 0; Y < GridHeight; ++Y)
			{
				for (int32 X = 0; X < GridWidth; ++X)
				{
					FCellState Cell = Subsystem->GetCellAt(X, Y);
					float NormalizedValue = Cell.bExists ? (Cell.MantleEnergy / MaxEnergy) : 0.0f;
					HeatmapData.Add(FMath::Clamp(NormalizedValue, 0.0f, 1.0f));
				}
			}
		}
		break;

	case EHeatmapDataType::Temperature:
		{
			// 温度热力图（归一化到0-1）
			const float MinTemp = -50.0f;
			const float MaxTemp = 50.0f;
			for (int32 Y = 0; Y < GridHeight; ++Y)
			{
				for (int32 X = 0; X < GridWidth; ++X)
				{
					FCellState Cell = Subsystem->GetCellAt(X, Y);
					float NormalizedValue = Cell.bExists ? 
						((Cell.Temperature - MinTemp) / (MaxTemp - MinTemp)) : 0.0f;
					HeatmapData.Add(FMath::Clamp(NormalizedValue, 0.0f, 1.0f));
				}
			}
		}
		break;

	case EHeatmapDataType::CrystalDensity:
		{
			// 晶石密度（1=有晶石，0=无晶石）
			for (int32 Y = 0; Y < GridHeight; ++Y)
			{
				for (int32 X = 0; X < GridWidth; ++X)
				{
					FCellState Cell = Subsystem->GetCellAt(X, Y);
					float Value = (Cell.CrystalType != ECrystalType::Empty) ? 1.0f : 0.0f;
					HeatmapData.Add(Value);
				}
			}
		}
		break;

	case EHeatmapDataType::HumanDensity:
		{
			// 人类密度（繁荣度归一化）
			const float MaxProsperity = 100.0f;
			for (int32 Y = 0; Y < GridHeight; ++Y)
			{
				for (int32 X = 0; X < GridWidth; ++X)
				{
					FCellState Cell = Subsystem->GetCellAt(X, Y);
					float NormalizedValue = Cell.Prosperity / MaxProsperity;
					HeatmapData.Add(FMath::Clamp(NormalizedValue, 0.0f, 1.0f));
				}
			}
		}
		break;

	default:
		break;
	}

	return HeatmapData;
}
