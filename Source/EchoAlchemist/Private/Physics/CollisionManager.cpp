// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/CollisionManager.h"

void UCollisionManager::Initialize(FVector BoundsMin, FVector BoundsMax, float InCellSize)
{
	// 清空现有数据
	Bodies.Empty();
	
	// 保存配置
	Bounds = FBox(BoundsMin, BoundsMax);
	CellSize = InCellSize;
	
	// 创建空间网格
	SpatialGrid = MakeUnique<FSpatialGrid>(Bounds, CellSize);
	
	// 重置游戏时间
	CurrentGameTime = 0.0f;
	
	// 标记为已初始化
	bIsInitialized = true;
	
	UE_LOG(LogTemp, Log, TEXT("[CollisionManager] Initialized: Bounds=%s, CellSize=%.2f"),
		*Bounds.ToString(), CellSize);
}

void UCollisionManager::Cleanup()
{
	Bodies.Empty();
	SpatialGrid.Reset();
	bIsInitialized = false;
	CurrentGameTime = 0.0f;
	
	UE_LOG(LogTemp, Log, TEXT("[CollisionManager] Cleaned up"));
}

FGuid UCollisionManager::RegisterBody(const FCollisionBody& Body)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("[CollisionManager] Cannot register body: System not initialized"));
		return FGuid();
	}
	
	FGuid BodyID = Body.ID;
	Bodies.Add(BodyID, Body);
	
	UE_LOG(LogTemp, Verbose, TEXT("[CollisionManager] Body registered: ID=%s, Type=%s"),
		*BodyID.ToString(),
		*UEnum::GetValueAsString(Body.ShapeType));
	
	return BodyID;
}

bool UCollisionManager::UnregisterBody(const FGuid& BodyID)
{
	if (Bodies.Remove(BodyID) > 0)
	{
		UE_LOG(LogTemp, Verbose, TEXT("[CollisionManager] Body unregistered: ID=%s"), *BodyID.ToString());
		return true;
	}
	
	return false;
}

bool UCollisionManager::UpdateBodyPosition(const FGuid& BodyID, FVector NewPosition)
{
	FCollisionBody* Body = Bodies.Find(BodyID);
	if (Body)
	{
		Body->Position = NewPosition;
		return true;
	}
	
	return false;
}

bool UCollisionManager::GetBody(const FGuid& BodyID, FCollisionBody& OutBody) const
{
	const FCollisionBody* Found = Bodies.Find(BodyID);
	if (Found)
	{
		OutBody = *Found;
		return true;
	}
	
	return false;
}

TArray<FCollisionBody> UCollisionManager::GetAllBodies() const
{
	TArray<FCollisionBody> Result;
	Bodies.GenerateValueArray(Result);
	return Result;
}

int32 UCollisionManager::GetBodyCount() const
{
	return Bodies.Num();
}

void UCollisionManager::UpdateSpatialGrid()
{
	if (!bIsInitialized || !SpatialGrid.IsValid())
	{
		return;
	}
	
	// 清空网格
	SpatialGrid->Clear();
	
	// 重新插入所有碰撞体
	for (const auto& Pair : Bodies)
	{
		SpatialGrid->Insert(Pair.Value);
	}
}

void UCollisionManager::GetSpatialGridStatistics(int32& OutTotalCells, int32& OutOccupiedCells, 
                                                  int32& OutMaxBodiesPerCell, float& OutAvgBodiesPerCell) const
{
	if (SpatialGrid.IsValid())
	{
		SpatialGrid->GetStatistics(OutTotalCells, OutOccupiedCells, OutMaxBodiesPerCell, OutAvgBodiesPerCell);
	}
	else
	{
		OutTotalCells = 0;
		OutOccupiedCells = 0;
		OutMaxBodiesPerCell = 0;
		OutAvgBodiesPerCell = 0.0f;
	}
}

TArray<FEchoCollisionEvent> UCollisionManager::DetectCollisions()
{
	TArray<FEchoCollisionEvent> Collisions;
	
	if (!bIsInitialized || !SpatialGrid.IsValid())
	{
		return Collisions;
	}
	
	// 使用集合记录已检测的碰撞对，避免重复检测
	TSet<TPair<FGuid, FGuid>> CheckedPairs;
	
	// 遍历所有碰撞体
	for (const auto& PairA : Bodies)
	{
		const FCollisionBody& BodyA = PairA.Value;
		
		// 查询空间网格，获取可能碰撞的碰撞体
		TArray<FCollisionBody> NearbyBodies;
		FBox QueryBounds = BodyA.GetBoundingBox();
		SpatialGrid->QueryBox(QueryBounds, NearbyBodies);
		
		// 检测碰撞
		for (const FCollisionBody& BodyB : NearbyBodies)
		{
			// 跳过自己
			if (BodyA.ID == BodyB.ID)
			{
				continue;
			}
			
			// 跳过已检测的碰撞对
			TPair<FGuid, FGuid> Pair1(BodyA.ID, BodyB.ID);
			TPair<FGuid, FGuid> Pair2(BodyB.ID, BodyA.ID);
			if (CheckedPairs.Contains(Pair1) || CheckedPairs.Contains(Pair2))
			{
				continue;
			}
			CheckedPairs.Add(Pair1);
			
			// 执行碰撞检测
			FEchoCollisionEvent Event;
			if (CheckCollision(BodyA, BodyB, Event))
			{
				Event.Timestamp = CurrentGameTime;
				Collisions.Add(Event);
				
				// 触发事件
				OnCollision.Broadcast(Event);
			}
		}
	}
	
	return Collisions;
}

int32 UCollisionManager::DetectCollisionsForBody(const FGuid& BodyID, TArray<FEchoCollisionEvent>& OutCollisions)
{
	OutCollisions.Empty();
	
	if (!bIsInitialized || !SpatialGrid.IsValid())
	{
		return 0;
	}
	
	// 获取碰撞体
	const FCollisionBody* BodyA = Bodies.Find(BodyID);
	if (!BodyA)
	{
		return 0;
	}
	
	// 查询空间网格
	TArray<FCollisionBody> NearbyBodies;
	FBox QueryBounds = BodyA->GetBoundingBox();
	SpatialGrid->QueryBox(QueryBounds, NearbyBodies);
	
	// 检测碰撞
	for (const FCollisionBody& BodyB : NearbyBodies)
	{
		// 跳过自己
		if (BodyA->ID == BodyB.ID)
		{
			continue;
		}
		
		// 执行碰撞检测
		FEchoCollisionEvent Event;
		if (CheckCollision(*BodyA, BodyB, Event))
		{
			Event.Timestamp = CurrentGameTime;
			OutCollisions.Add(Event);
			
			// 触发事件
			OnCollision.Broadcast(Event);
		}
	}
	
	return OutCollisions.Num();
}

bool UCollisionManager::CheckCollision(const FCollisionBody& BodyA, const FCollisionBody& BodyB, FEchoCollisionEvent& OutEvent)
{
	// 根据形状类型分发到具体的碰撞检测函数
	if (BodyA.ShapeType == EEchoCollisionShapeType::Circle && BodyB.ShapeType == EEchoCollisionShapeType::Circle)
	{
		return CheckCircleCircle(BodyA, BodyB, OutEvent);
	}
	else if (BodyA.ShapeType == EEchoCollisionShapeType::Circle && BodyB.ShapeType == EEchoCollisionShapeType::Rectangle)
	{
		return CheckCircleRectangle(BodyA, BodyB, OutEvent);
	}
	else if (BodyA.ShapeType == EEchoCollisionShapeType::Rectangle && BodyB.ShapeType == EEchoCollisionShapeType::Circle)
	{
		return CheckCircleRectangle(BodyB, BodyA, OutEvent);
	}
	
	// 矩形-矩形碰撞暂不支持
	return false;
}

bool UCollisionManager::CheckCircleCircle(const FCollisionBody& BodyA, const FCollisionBody& BodyB, FEchoCollisionEvent& OutEvent)
{
	// 计算距离
	FVector Delta = BodyB.Position - BodyA.Position;
	float Distance = Delta.Size();
	
	// 计算半径和
	float RadiusSum = BodyA.Radius + BodyB.Radius;
	
	// 检查是否碰撞
	if (Distance < RadiusSum)
	{
		// 填充碰撞事件
		OutEvent.BodyA = BodyA.ID;
		OutEvent.BodyB = BodyB.ID;
		OutEvent.PenetrationDepth = RadiusSum - Distance;
		
		// 计算碰撞法线（从A指向B）
		if (Distance > KINDA_SMALL_NUMBER)
		{
			OutEvent.HitNormal = Delta / Distance;
		}
		else
		{
			// 距离太小，使用默认法线
			OutEvent.HitNormal = FVector(1, 0, 0);
		}
		
		// 计算碰撞点（两个圆心连线上，距离A的半径处）
		OutEvent.HitPoint = BodyA.Position + OutEvent.HitNormal * BodyA.Radius;
		
		return true;
	}
	
	return false;
}

bool UCollisionManager::CheckCircleRectangle(const FCollisionBody& Circle, const FCollisionBody& Rectangle, FEchoCollisionEvent& OutEvent)
{
	// 简化版本：将矩形视为轴对齐边界盒（AABB），不考虑旋转
	// TODO: 实现支持旋转的矩形碰撞检测
	
	// 计算矩形的边界
	FVector RectMin = Rectangle.Position - FVector(Rectangle.Size.X / 2.0f, Rectangle.Size.Y / 2.0f, 0.0f);
	FVector RectMax = Rectangle.Position + FVector(Rectangle.Size.X / 2.0f, Rectangle.Size.Y / 2.0f, 0.0f);
	
	// 找到矩形上距离圆心最近的点
	FVector ClosestPoint;
	ClosestPoint.X = FMath::Clamp(Circle.Position.X, RectMin.X, RectMax.X);
	ClosestPoint.Y = FMath::Clamp(Circle.Position.Y, RectMin.Y, RectMax.Y);
	ClosestPoint.Z = Circle.Position.Z;  // 2D碰撞，Z轴不变
	
	// 计算距离
	FVector Delta = Circle.Position - ClosestPoint;
	float Distance = Delta.Size();
	
	// 检查是否碰撞
	if (Distance < Circle.Radius)
	{
		// 填充碰撞事件
		OutEvent.BodyA = Circle.ID;
		OutEvent.BodyB = Rectangle.ID;
		OutEvent.PenetrationDepth = Circle.Radius - Distance;
		OutEvent.HitPoint = ClosestPoint;
		
		// 计算碰撞法线
		if (Distance > KINDA_SMALL_NUMBER)
		{
			OutEvent.HitNormal = Delta / Distance;
		}
		else
		{
			// 圆心在矩形内部，使用从矩形中心指向圆心的方向
			FVector ToCircle = Circle.Position - Rectangle.Position;
			if (ToCircle.SizeSquared() > KINDA_SMALL_NUMBER)
			{
				OutEvent.HitNormal = ToCircle.GetSafeNormal();
			}
			else
			{
				OutEvent.HitNormal = FVector(1, 0, 0);
			}
		}
		
		return true;
	}
	
	return false;
}
