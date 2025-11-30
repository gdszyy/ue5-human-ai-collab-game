// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldMorphing/WorldMorphingSimulation.h"
#include "WorldMorphing/WorldMorphingSubsystem.h"
#include "Engine/GameInstance.h"

static UWorldMorphingSubsystem* GetWorldMorphingSubsystem(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
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

bool UWorldMorphingSimulation::Initialize(UObject* WorldContextObject, 
                                           int32 Width, 
                                           int32 Height, 
                                           const FSimulationParams& Params)
{
	UWorldMorphingSubsystem* Subsystem = GetWorldMorphingSubsystem(WorldContextObject);
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("WorldMorphingSimulation::Initialize - Failed to get subsystem"));
		return false;
	}

	Subsystem->InitializeWorld(Width, Height, Params);
	return true;
}

void UWorldMorphingSimulation::Tick(UObject* WorldContextObject, float DeltaTime)
{
	UWorldMorphingSubsystem* Subsystem = GetWorldMorphingSubsystem(WorldContextObject);
	if (!Subsystem)
	{
		return;
	}

	Subsystem->TickSimulation(DeltaTime);
}

void UWorldMorphingSimulation::Reset(UObject* WorldContextObject)
{
	UWorldMorphingSubsystem* Subsystem = GetWorldMorphingSubsystem(WorldContextObject);
	if (!Subsystem)
	{
		return;
	}

	// 获取当前网格尺寸和参数
	int32 CurrentWidth, CurrentHeight;
	Subsystem->GetGridSize(CurrentWidth, CurrentHeight);
	FSimulationParams CurrentParams = Subsystem->GetSimulationParams();

	// 重新初始化
	Subsystem->InitializeWorld(CurrentWidth, CurrentHeight, CurrentParams);
}

void UWorldMorphingSimulation::SetPaused(UObject* WorldContextObject, bool bPause)
{
	// 暂停功能需要在Subsystem中添加支持
	// 这里先留空，后续可以扩展
	UE_LOG(LogTemp, Warning, TEXT("WorldMorphingSimulation::SetPaused - Not implemented yet"));
}

FSimulationStatus UWorldMorphingSimulation::GetStatus(UObject* WorldContextObject)
{
	FSimulationStatus Status;

	UWorldMorphingSubsystem* Subsystem = GetWorldMorphingSubsystem(WorldContextObject);
	if (!Subsystem)
	{
		return Status;
	}

	// 填充状态信息
	Subsystem->GetGridSize(Status.Width, Status.Height);
	Status.TimeStep = Subsystem->GetTimeStep();
	Status.CycleCount = Subsystem->GetCycleCount();
	Status.bInitialized = (Status.Width > 0 && Status.Height > 0);
	Status.bPaused = false; // 暂时固定为false

	return Status;
}
