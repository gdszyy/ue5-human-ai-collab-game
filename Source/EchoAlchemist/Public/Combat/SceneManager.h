// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SceneManager.generated.h"

/**
 * 场景管理器接口
 * 负责场景适配，支持下落式和环形两种战斗场景
 * 
 * 设计理念：
 * - 策略模式：通过接口实现不同场景的统一抽象
 * - 坐标转换：提供逻辑坐标和屏幕坐标的转换
 * - 行为适配：提供不同场景的行为适配方法
 * 
 * 使用场景：
 * - 下落式场景：传统笛卡尔坐标系，支持重力
 * - 环形场景：极坐标系，无重力，敌人在圆环上移动
 */
UINTERFACE(MinimalAPI, Blueprintable)
class USceneManager : public UInterface
{
	GENERATED_BODY()
};

class ECHOALCHEMIST_API ISceneManager
{
	GENERATED_BODY()

public:
	// ========== 场景信息 ==========
	
	/**
	 * 获取场景类型
	 * @return 场景类型（"Falling" 或 "Circular"）
	 */
	virtual FString GetSceneType() const = 0;

	// ========== 行为适配 ==========
	
	/**
	 * 更新位置和速度
	 * @param Position 位置（输入/输出）
	 * @param Velocity 速度（输入/输出）
	 * @param DeltaTime 时间增量（秒）
	 */
	virtual void Advance(FVector& Position, FVector& Velocity, float DeltaTime) = 0;

	/**
	 * 获取发射向量
	 * @param StartPosition 起始位置
	 * @param TargetAngle 目标角度（度）
	 * @return 发射向量（归一化）
	 */
	virtual FVector GetLaunchVector(const FVector& StartPosition, float TargetAngle) = 0;

	// ========== 边界与环境 ==========
	
	/**
	 * 检查是否超出边界
	 * @param Position 位置
	 * @return 是否超出边界
	 */
	virtual bool IsOutOfBounds(const FVector& Position) const = 0;

	/**
	 * 应用环境力（如重力）
	 * @param Velocity 速度（输入/输出）
	 * @param DeltaTime 时间增量（秒）
	 */
	virtual void ApplyEnvironmentalForces(FVector& Velocity, float DeltaTime) = 0;

	/**
	 * 处理边界交互（如反弹）
	 * @param Position 位置（输入/输出）
	 * @param Velocity 速度（输入/输出）
	 */
	virtual void HandleBoundaryInteraction(FVector& Position, FVector& Velocity) = 0;

	// ========== 坐标转换 ==========
	
	/**
	 * 逻辑坐标转屏幕坐标
	 * @param LogicalPosition 逻辑坐标
	 * @return 屏幕坐标
	 */
	virtual FVector WorldToScreen(const FVector& LogicalPosition) const = 0;

	/**
	 * 屏幕坐标转逻辑坐标
	 * @param ScreenPosition 屏幕坐标
	 * @return 逻辑坐标
	 */
	virtual FVector ScreenToWorld(const FVector& ScreenPosition) const = 0;
};
