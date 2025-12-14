// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Physics/CollisionShape.h"
#include "Physics/SpatialGrid.h"
#include "CollisionManager.generated.h"

/**
 * 碰撞事件委托
 * 
 * 当发生碰撞时触发此委托。
 * 
 * 蓝图使用示例：
 * ```
 * // 绑定碰撞事件
 * CollisionManager->OnCollision.AddDynamic(this, &AMyActor::HandleCollision);
 * 
 * // 事件处理函数
 * void AMyActor::HandleCollision(const FEchoCollisionEvent& Event)
 * {
 *     // 处理碰撞
 * }
 * ```
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollisionDelegate, const FEchoCollisionEvent&, CollisionEvent);

/**
 * 碰撞管理器
 * 
 * 负责管理所有碰撞体并执行碰撞检测。
 * 使用空间网格优化性能，支持圆-圆和圆-矩形碰撞检测。
 * 
 * 蓝图使用示例：
 * 
 * 1. 初始化
 *    ```
 *    CollisionManager = NewObject<UCollisionManager>();
 *    CollisionManager->Initialize(FVector(-1000, -1000, 0), FVector(1000, 1000, 1000), 100.0f);
 *    ```
 * 
 * 2. 注册碰撞体
 *    ```
 *    FCollisionBody Body;
 *    Body.Position = GetActorLocation();
 *    Body.ShapeType = CollisionShape_Circle;
 *    Body.Radius = 10.0f;
 *    CollisionManager->RegisterBody(Body);
 *    ```
 * 
 * 3. 更新碰撞体位置
 *    ```
 *    CollisionManager->UpdateBodyPosition(BodyID, NewPosition);
 *    ```
 * 
 * 4. 执行碰撞检测
 *    ```
 *    TArray<FEchoCollisionEvent> Collisions = CollisionManager->DetectCollisions();
 *    for (const FEchoCollisionEvent& Event : Collisions)
 *    {
 *        // 处理碰撞
 *    }
 *    ```
 * 
 * 注意事项：
 * - 必须先调用Initialize才能使用其他功能
 * - 碰撞体位置更新后需要调用UpdateSpatialGrid重建空间网格
 * - 碰撞检测不会自动触发事件，需要手动调用DetectCollisions
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API UCollisionManager : public UObject
{
	GENERATED_BODY()

public:
	// ========== 初始化和清理 ==========
	
	/**
	 * 初始化碰撞管理器
	 * 
	 * @param BoundsMin 空间边界最小点
	 * @param BoundsMax 空间边界最大点
	 * @param CellSize 空间网格单元尺寸（单位：cm，建议设置为碰撞体平均大小的2-3倍）
	 * 
	 * 注意事项：
	 * - 必须在使用其他功能前调用
	 * - 会清空所有现有的碰撞体
	 * - 可以重复调用来重新初始化
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	void Initialize(FVector BoundsMin, FVector BoundsMax, float CellSize = 100.0f);

	/**
	 * 清理碰撞管理器
	 * 
	 * 删除所有碰撞体并重置系统状态。
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	void Cleanup();

	// ========== 碰撞体管理 ==========
	
	/**
	 * 注册新的碰撞体
	 * 
	 * @param Body 碰撞体数据
	 * @return 碰撞体的唯一ID
	 * 
	 * 注意事项：
	 * - 返回的ID可用于后续更新或删除
	 * - 注册后需要调用UpdateSpatialGrid更新空间网格
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	FGuid RegisterBody(const FCollisionBody& Body);

	/**
	 * 注销碰撞体
	 * 
	 * @param BodyID 碰撞体ID
	 * @return true=注销成功，false=ID不存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	bool UnregisterBody(const FGuid& BodyID);

	/**
	 * 更新碰撞体位置
	 * 
	 * @param BodyID 碰撞体ID
	 * @param NewPosition 新位置
	 * @return true=更新成功，false=ID不存在
	 * 
	 * 注意事项：
	 * - 更新后需要调用UpdateSpatialGrid更新空间网格
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	bool UpdateBodyPosition(const FGuid& BodyID, FVector NewPosition);

	/**
	 * 获取碰撞体数据
	 * 
	 * @param BodyID 碰撞体ID
	 * @param OutBody 输出参数，存储碰撞体数据
	 * @return true=获取成功，false=ID不存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	bool GetBody(const FGuid& BodyID, FCollisionBody& OutBody) const;

	/**
	 * 获取所有碰撞体
	 * 
	 * @return 所有碰撞体的数组
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	TArray<FCollisionBody> GetAllBodies() const;

	/**
	 * 获取碰撞体数量
	 * 
	 * @return 当前注册的碰撞体数量
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Collision")
	int32 GetBodyCount() const;

	// ========== 空间网格 ==========
	
	/**
	 * 更新空间网格
	 * 
	 * 在碰撞体位置更新后调用，重建空间网格以保持碰撞检测性能。
	 * 
	 * 注意事项：
	 * - 每帧调用一次即可，不需要每次更新位置都调用
	 * - 如果碰撞体数量很大，此操作可能耗时较长
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	void UpdateSpatialGrid();

	/**
	 * 获取空间网格统计信息（用于调试和性能分析）
	 * 
	 * @param OutTotalCells 总网格数
	 * @param OutOccupiedCells 已占用网格数
	 * @param OutMaxBodiesPerCell 单个网格最大碰撞体数
	 * @param OutAvgBodiesPerCell 单个网格平均碰撞体数
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision|Debug")
	void GetSpatialGridStatistics(int32& OutTotalCells, int32& OutOccupiedCells, 
	                               int32& OutMaxBodiesPerCell, float& OutAvgBodiesPerCell) const;

	// ========== 碰撞检测 ==========
	
	/**
	 * 执行碰撞检测
	 * 
	 * @return 所有检测到的碰撞事件
	 * 
	 * 注意事项：
	 * - 会自动触发OnCollision事件
	 * - 建议每帧调用一次
	 * - 使用空间网格优化，性能为O(n * k)
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	TArray<FEchoCollisionEvent> DetectCollisions();

	/**
	 * 检测指定碰撞体与其他碰撞体的碰撞
	 * 
	 * @param BodyID 要检测的碰撞体ID
	 * @param OutCollisions 输出参数，存储碰撞事件
	 * @return 碰撞数量
	 * 
	 * 使用场景：
	 * - 只关心特定碰撞体的碰撞
	 * - 减少不必要的碰撞检测
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Collision")
	int32 DetectCollisionsForBody(const FGuid& BodyID, TArray<FEchoCollisionEvent>& OutCollisions);

	// ========== 碰撞事件 ==========
	
	/** 碰撞事件委托 */
	UPROPERTY(BlueprintAssignable, Category = "Physics|Collision|Events")
	FOnCollisionDelegate OnCollision;

	// ========== 查询 ==========
	
	/**
	 * 检查系统是否已初始化
	 * 
	 * @return true=已初始化，false=未初始化
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Collision")
	bool IsInitialized() const { return bIsInitialized; }

private:
	// ========== 内部状态 ==========
	
	/** 是否已初始化 */
	bool bIsInitialized = false;

	/** 空间边界 */
	FBox Bounds;

	/** 空间网格单元尺寸 */
	float CellSize = 100.0f;

	/** 碰撞体映射表（ID -> Body） */
	TMap<FGuid, FCollisionBody> Bodies;

	/** 空间网格 */
	TUniquePtr<FSpatialGrid> SpatialGrid;

	/** 当前游戏时间（单位：秒） */
	float CurrentGameTime = 0.0f;

	// ========== 碰撞检测算法 ==========
	
	/**
	 * 检测两个碰撞体是否碰撞
	 * 
	 * @param BodyA 碰撞体A
	 * @param BodyB 碰撞体B
	 * @param OutEvent 输出参数，存储碰撞事件
	 * @return true=发生碰撞，false=未碰撞
	 */
	bool CheckCollision(const FCollisionBody& BodyA, const FCollisionBody& BodyB, FEchoCollisionEvent& OutEvent);

	/**
	 * 圆-圆碰撞检测
	 * 
	 * @param BodyA 圆形碰撞体A
	 * @param BodyB 圆形碰撞体B
	 * @param OutEvent 输出参数，存储碰撞事件
	 * @return true=发生碰撞，false=未碰撞
	 */
	bool CheckCircleCircle(const FCollisionBody& BodyA, const FCollisionBody& BodyB, FEchoCollisionEvent& OutEvent);

	/**
	 * 圆-矩形碰撞检测
	 * 
	 * @param Circle 圆形碰撞体
	 * @param Rectangle 矩形碰撞体
	 * @param OutEvent 输出参数，存储碰撞事件
	 * @return true=发生碰撞，false=未碰撞
	 */
	bool CheckCircleRectangle(const FCollisionBody& Circle, const FCollisionBody& Rectangle, FEchoCollisionEvent& OutEvent);
};
