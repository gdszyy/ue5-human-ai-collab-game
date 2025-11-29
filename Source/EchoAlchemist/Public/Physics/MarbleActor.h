// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Physics/MarbleState.h"
#include "MarbleActor.generated.h"

/**
 * 魔力露珠Actor
 * 
 * 高精度物理实现，用于核心弹珠（第0-1代）。
 * 使用UE4原生物理引擎进行模拟，提供最高的物理精度。
 * 
 * 蓝图使用示例：
 * 
 * 1. 生成魔力露珠Actor
 *    ```
 *    FActorSpawnParameters SpawnParams;
 *    AMarbleActor* Marble = GetWorld()->SpawnActor<AMarbleActor>(
 *        AMarbleActor::StaticClass(),
 *        SpawnLocation,
 *        FRotator::ZeroRotator,
 *        SpawnParams
 *    );
 *    ```
 * 
 * 2. 初始化魔力露珠
 *    ```
 *    FMarbleState State;
 *    State.Position = SpawnLocation;
 *    State.Velocity = LaunchDirection * LaunchSpeed;
 *    State.Radius = 10.0f;
 *    Marble->InitializeFromState(State);
 *    ```
 * 
 * 3. 发射魔力露珠
 *    ```
 *    Marble->Launch(LaunchDirection, LaunchSpeed);
 *    ```
 * 
 * 4. 获取当前状态
 *    ```
 *    FMarbleState CurrentState = Marble->GetCurrentState();
 *    ```
 * 
 * 注意事项：
 * - 必须调用InitializeFromState才能正确设置物理属性
 * - 物理模拟由UE4引擎自动处理，无需手动更新
 * - 使用对象池管理Actor以提升性能
 */
UCLASS()
class ECHOALCHEMIST_API AMarbleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMarbleActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// ========== 初始化 ==========
	
	/**
	 * 从魔力露珠状态初始化Actor
	 * 
	 * @param State 魔力露珠状态
	 * 
	 * 注意事项：
	 * - 必须在生成Actor后立即调用
	 * - 会设置位置、速度、半径、质量等物理属性
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	void InitializeFromState(const FMarbleState& State);

	/**
	 * 发射魔力露珠
	 * 
	 * @param Direction 发射方向（会自动归一化）
	 * @param Speed 发射速度（单位：cm/s）
	 * 
	 * 注意事项：
	 * - 会立即应用冲量到物理组件
	 * - 如果已经在运动中，会叠加速度
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	void Launch(FVector Direction, float Speed);

	// ========== 状态查询 ==========
	
	/**
	 * 获取当前魔力露珠状态
	 * 
	 * @return 当前状态
	 * 
	 * 注意事项：
	 * - 返回的是实时状态，包含当前位置、速度等
	 * - 可以用于同步到物理系统
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Marble")
	FMarbleState GetCurrentState() const;

	/**
	 * 获取魔力露珠ID
	 * 
	 * @return 唯一标识符
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Marble")
	FGuid GetMarbleID() const { return MarbleID; }

	/**
	 * 获取当前速度
	 * 
	 * @return 速度向量（单位：cm/s）
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Marble")
	FVector GetVelocity() const;

	/**
	 * 获取当前速率
	 * 
	 * @return 速率标量（单位：cm/s）
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Marble")
	float GetSpeed() const;

	// ========== 物理控制 ==========
	
	/**
	 * 设置重力启用状态
	 * 
	 * @param bEnable true=启用重力，false=禁用重力
	 * 
	 * 使用场景：
	 * - 炼金工作台：启用重力
	 * - 战斗场景：禁用重力
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	void SetGravityEnabled(bool bEnable);

	/**
	 * 应用冲量
	 * 
	 * @param Impulse 冲量向量（单位：kg*cm/s）
	 * 
	 * 注意事项：
	 * - 会立即改变速度
	 * - 冲量 = 质量 * 速度变化
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	void ApplyImpulse(FVector Impulse);

	// ========== 对象池 ==========
	
	/**
	 * 重置Actor到初始状态
	 * 
	 * 用于对象池回收时重置Actor。
	 * 
	 * 注意事项：
	 * - 会停止所有物理模拟
	 * - 会隐藏Actor
	 * - 会重置所有状态
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Marble")
	void ResetToPool();

	/**
	 * 检查Actor是否处于对象池中
	 * 
	 * @return true=在对象池中，false=正在使用
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Marble")
	bool IsInPool() const { return bIsInPool; }

protected:
	// ========== 组件 ==========
	
	/** 球体碰撞组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	/** 静态网格组件（视觉表现） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	// ========== 状态 ==========
	
	/** 魔力露珠唯一标识符 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FGuid MarbleID;

	/** 是否在对象池中 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsInPool = false;

	/** 魔力露珠状态（用于同步） */
	FMarbleState CachedState;
};
