// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/FallingSceneManager.h"

UFallingSceneManager::UFallingSceneManager()
{
	SceneWidth = 800.0f;
	SceneHeight = 600.0f;
	Gravity = FVector(0, 0, -980.0f);
	BounceCoefficient = 0.8f;
}

void UFallingSceneManager::Initialize(float Width, float Height)
{
	SceneWidth = Width;
	SceneHeight = Height;
}

void UFallingSceneManager::SetGravity(FVector InGravity)
{
	Gravity = InGravity;
}

void UFallingSceneManager::Advance(FVector& Position, FVector& Velocity, float DeltaTime)
{
	// 应用速度
	Position += Velocity * DeltaTime;
	
	// 应用重力
	ApplyEnvironmentalForces(Velocity, DeltaTime);
	
	// 处理边界交互
	HandleBoundaryInteraction(Position, Velocity);
}

FVector UFallingSceneManager::GetLaunchVector(const FVector& StartPosition, float TargetAngle)
{
	// 下落式场景使用标准的2D角度
	// 0度 = 右，90度 = 上，180度 = 左，270度 = 下
	const float RadianAngle = FMath::DegreesToRadians(TargetAngle);
	const float X = FMath::Cos(RadianAngle);
	const float Z = FMath::Sin(RadianAngle);
	
	return FVector(X, 0, Z).GetSafeNormal();
}

bool UFallingSceneManager::IsOutOfBounds(const FVector& Position) const
{
	const float HalfWidth = SceneWidth * 0.5f;
	const float HalfHeight = SceneHeight * 0.5f;
	
	return Position.X < -HalfWidth || Position.X > HalfWidth ||
	       Position.Z < -HalfHeight || Position.Z > HalfHeight;
}

void UFallingSceneManager::ApplyEnvironmentalForces(FVector& Velocity, float DeltaTime)
{
	// 应用重力
	Velocity += Gravity * DeltaTime;
}

void UFallingSceneManager::HandleBoundaryInteraction(FVector& Position, FVector& Velocity)
{
	const float HalfWidth = SceneWidth * 0.5f;
	const float HalfHeight = SceneHeight * 0.5f;
	
	// 左右边界
	if (Position.X < -HalfWidth)
	{
		Position.X = -HalfWidth;
		Velocity.X = -Velocity.X * BounceCoefficient;
	}
	else if (Position.X > HalfWidth)
	{
		Position.X = HalfWidth;
		Velocity.X = -Velocity.X * BounceCoefficient;
	}
	
	// 上下边界
	if (Position.Z < -HalfHeight)
	{
		Position.Z = -HalfHeight;
		Velocity.Z = -Velocity.Z * BounceCoefficient;
	}
	else if (Position.Z > HalfHeight)
	{
		Position.Z = HalfHeight;
		Velocity.Z = -Velocity.Z * BounceCoefficient;
	}
}

FVector UFallingSceneManager::WorldToScreen(const FVector& LogicalPosition) const
{
	// 下落式场景的逻辑坐标和屏幕坐标相同
	return LogicalPosition;
}

FVector UFallingSceneManager::ScreenToWorld(const FVector& ScreenPosition) const
{
	// 下落式场景的屏幕坐标和逻辑坐标相同
	return ScreenPosition;
}
