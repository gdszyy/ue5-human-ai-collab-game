// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/MarbleActorPool.h"
#include "Engine/World.h"

void UMarbleActorPool::Initialize(UWorld* World, int32 PreAllocateCount)
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[MarbleActorPool] Cannot initialize: World is null"));
		return;
	}

	// 清空现有数据
	Clear();

	// 保存世界对象
	CachedWorld = World;

	// 预创建Actor
	for (int32 i = 0; i < PreAllocateCount; ++i)
	{
		AMarbleActor* NewActor = CreateNewActor();
		if (NewActor)
		{
			AvailableActors.Add(NewActor);
		}
	}

	// 标记为已初始化
	bIsInitialized = true;

	UE_LOG(LogTemp, Log, TEXT("[MarbleActorPool] Initialized: PreAllocated=%d"), PreAllocateCount);
}

void UMarbleActorPool::Clear()
{
	// 销毁所有Actor
	for (AMarbleActor* Actor : AvailableActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

	for (AMarbleActor* Actor : InUseActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

	// 清空列表
	AvailableActors.Empty();
	InUseActors.Empty();

	// 重置状态
	bIsInitialized = false;
	CachedWorld = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[MarbleActorPool] Cleared"));
}

AMarbleActor* UMarbleActorPool::Acquire()
{
	if (!bIsInitialized || !CachedWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("[MarbleActorPool] Cannot acquire: Pool not initialized"));
		return nullptr;
	}

	AMarbleActor* Actor = nullptr;

	// 从可用列表中获取
	if (AvailableActors.Num() > 0)
	{
		Actor = AvailableActors.Pop();
	}
	else
	{
		// 池为空，创建新的Actor
		Actor = CreateNewActor();
		UE_LOG(LogTemp, Warning, TEXT("[MarbleActorPool] Pool empty, creating new actor"));
	}

	if (Actor)
	{
		// 移动到使用中列表
		InUseActors.Add(Actor);

		UE_LOG(LogTemp, Verbose, TEXT("[MarbleActorPool] Acquired: Available=%d, InUse=%d"),
			AvailableActors.Num(), InUseActors.Num());
	}

	return Actor;
}

void UMarbleActorPool::Release(AMarbleActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	// 从使用中列表移除
	InUseActors.Remove(Actor);

	// 重置Actor状态
	Actor->ResetToPool();

	// 添加到可用列表
	AvailableActors.Add(Actor);

	UE_LOG(LogTemp, Verbose, TEXT("[MarbleActorPool] Released: Available=%d, InUse=%d"),
		AvailableActors.Num(), InUseActors.Num());
}

void UMarbleActorPool::GetStatistics(int32& OutTotalCount, int32& OutAvailableCount, int32& OutInUseCount) const
{
	OutAvailableCount = AvailableActors.Num();
	OutInUseCount = InUseActors.Num();
	OutTotalCount = OutAvailableCount + OutInUseCount;
}

AMarbleActor* UMarbleActorPool::CreateNewActor()
{
	if (!CachedWorld)
	{
		return nullptr;
	}

	// 生成Actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMarbleActor* NewActor = CachedWorld->SpawnActor<AMarbleActor>(
		AMarbleActor::StaticClass(),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (NewActor)
	{
		// 立即重置到池状态
		NewActor->ResetToPool();
	}

	return NewActor;
}
