// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CollisionShape.generated.h"

/**
 * 碰撞体形状类型
 * 
 * 定义碰撞检测支持的形状类型。
 * 
 * 蓝图使用示例：
 * - 在创建碰撞体时选择形状类型
 * - Circle用于魔力露珠、魔药等圆形物体
 * - Rectangle用于敌人、障碍物等矩形物体
 */
UENUM(BlueprintType)
enum class ECollisionShapeType : uint8
{
	/** 圆形碰撞体 */
	Circle UMETA(DisplayName = "Circle"),
	
	/** 矩形碰撞体 */
	Rectangle UMETA(DisplayName = "Rectangle")
};

/**
 * 碰撞体基础信息
 * 
 * 用于碰撞检测的基础数据结构，包含位置、形状类型等信息。
 * 
 * 蓝图使用示例：
 * - 创建碰撞体时填充此结构
 * - 传递给CollisionManager进行碰撞检测
 */
USTRUCT(BlueprintType)
struct FCollisionBody
{
	GENERATED_BODY()

	/** 唯一标识符 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FGuid ID;

	/** 位置（世界坐标系，单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FVector Position = FVector::ZeroVector;

	/** 形状类型（圆形或矩形） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ECollisionShapeType ShapeType = ECollisionShapeType::Circle;

	// ========== 圆形参数 ==========
	
	/** 半径（单位：cm，仅圆形使用） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Circle", meta = (ClampMin = "0.1"))
	float Radius = 10.0f;

	// ========== 矩形参数 ==========
	
	/** 矩形尺寸（单位：cm，仅矩形使用） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Rectangle")
	FVector2D Size = FVector2D(20.0f, 20.0f);

	/** 矩形旋转角度（单位：度，仅矩形使用） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Rectangle")
	float Rotation = 0.0f;

	/** 构造函数 */
	FCollisionBody()
	{
		ID = FGuid::NewGuid();
	}

	/**
	 * 获取碰撞体的边界盒
	 * 
	 * @return 轴对齐边界盒（AABB）
	 */
	FBox GetBoundingBox() const
	{
		if (ShapeType == ECollisionShapeType::Circle)
		{
			FVector Extent(Radius, Radius, Radius);
			return FBox(Position - Extent, Position + Extent);
		}
		else
		{
			// 矩形的边界盒（简化处理，不考虑旋转）
			FVector Extent(Size.X / 2.0f, Size.Y / 2.0f, 0.0f);
			return FBox(Position - Extent, Position + Extent);
		}
	}
};

/**
 * 碰撞事件数据
 * 
 * 当发生碰撞时，通过此结构体传递碰撞信息。
 * 
 * 蓝图使用示例：
 * - 订阅碰撞事件后，会收到此结构体
 * - 读取BodyA和BodyB的ID来识别碰撞对象
 * - 使用HitPoint来显示碰撞特效
 */
USTRUCT(BlueprintType)
struct FCollisionEvent
{
	GENERATED_BODY()

	/** 碰撞体A的ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FGuid BodyA;

	/** 碰撞体B的ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FGuid BodyB;

	/** 碰撞点（世界坐标系，单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FVector HitPoint = FVector::ZeroVector;

	/** 碰撞法线（从A指向B） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FVector HitNormal = FVector::ZeroVector;

	/** 穿透深度（单位：cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float PenetrationDepth = 0.0f;

	/** 碰撞时间戳（游戏时间，单位：秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float Timestamp = 0.0f;
};
