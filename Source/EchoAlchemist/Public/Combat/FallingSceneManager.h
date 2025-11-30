// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneManager.h"
#include "FallingSceneManager.generated.h"

/**
 * 下落式场景管理器
 * 处理传统笛卡尔坐标系的战斗场景
 * 
 * 特点：
 * - 笛卡尔坐标系
 * - 支持重力
 * - 矩形边界
 * - 简单的物理模拟
 */
UCLASS(Blueprintable)
class ECHOALCHEMIST_API UFallingSceneManager : public UObject, public ISceneManager
{
	GENERATED_BODY()

public:
	// ========== 构造函数 ==========
	
	UFallingSceneManager();

	// ========== 初始化 ==========
	
	/**
	 * 初始化场景
	 * @param Width 场景宽度（cm）
	 * @param Height 场景高度（cm）
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Scene")
	void Initialize(float Width, float Height);

	/**
	 * 设置重力
	 * @param InGravity 重力向量（cm/s²）
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Scene")
	void SetGravity(FVector InGravity);

	// ========== ISceneManager接口实现 ==========
	
	virtual FString GetSceneType() const override { return TEXT("Falling"); }
	
	virtual void Advance(FVector& Position, FVector& Velocity, float DeltaTime) override;
	
	virtual FVector GetLaunchVector(const FVector& StartPosition, float TargetAngle) override;
	
	virtual bool IsOutOfBounds(const FVector& Position) const override;
	
	virtual void ApplyEnvironmentalForces(FVector& Velocity, float DeltaTime) override;
	
	virtual void HandleBoundaryInteraction(FVector& Position, FVector& Velocity) override;
	
	virtual FVector WorldToScreen(const FVector& LogicalPosition) const override;
	
	virtual FVector ScreenToWorld(const FVector& ScreenPosition) const override;

protected:
	// ========== 场景参数 ==========
	
	/** 场景宽度（cm） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	float SceneWidth = 800.0f;

	/** 场景高度（cm） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	float SceneHeight = 600.0f;

	/** 重力向量（cm/s²） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	FVector Gravity = FVector(0, 0, -980.0f);

	/** 边界反弹系数 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|Scene")
	float BounceCoefficient = 0.8f;
};
