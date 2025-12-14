// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Physics/SpecialEffectData.h"
#include "Physics/MarbleState.h"
#include "SpecialEffectSystem.generated.h"

/**
 * 特殊效果事件委托
 * 
 * 当特殊效果触发时调用此委托。
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpecialEffectTriggeredDelegate, EEchoSpecialEffectType, EffectType, FGuid, TargetMarbleID);

/**
 * 特殊效果系统
 * 
 * 管理所有特殊物理效果，包括引力奇点、虫洞传送、弹珠分裂、速度修改和连锁触发。
 * 
 * 蓝图使用示例：
 * 
 * 1. 初始化
 *    ```
 *    EffectSystem = NewObject<USpecialEffectSystem>();
 *    EffectSystem->Initialize();
 *    ```
 * 
 * 2. 创建引力奇点
 *    ```
 *    FGravityWellParams Params;
 *    Params.Position = FVector(0, 0, 100);
 *    Params.GravityStrength = 1000.0f;
 *    Params.EffectRadius = 500.0f;
 *    Params.Duration = 5.0f;
 *    
 *    FGuid EffectID = EffectSystem->CreateGravityWell(Params);
 *    ```
 * 
 * 3. 创建虫洞
 *    ```
 *    FWormholeParams Params;
 *    Params.EntrancePosition = FVector(0, 0, 100);
 *    Params.ExitPosition = FVector(500, 0, 100);
 *    Params.EntranceRadius = 50.0f;
 *    
 *    FGuid EffectID = EffectSystem->CreateWormhole(Params);
 *    ```
 * 
 * 4. 应用分裂效果
 *    ```
 *    FSplitParams Params;
 *    Params.SplitCount = 3;
 *    Params.SplitAngle = 30.0f;
 *    
 *    TArray<FMarbleState> NewMarbles = EffectSystem->ApplySplitEffect(MarbleID, Params);
 *    ```
 * 
 * 5. 更新效果（在Tick中）
 *    ```
 *    EffectSystem->Tick(DeltaTime, AllMarbles);
 *    ```
 * 
 * 注意事项：
 * - 必须每帧调用Tick来更新效果
 * - 效果会自动根据Duration参数过期
 * - 分裂和连锁触发会返回新的弹珠状态，需要添加到物理系统
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API USpecialEffectSystem : public UObject
{
	GENERATED_BODY()

public:
	// ========== 初始化和清理 ==========
	
	/**
	 * 初始化特殊效果系统
	 * 
	 * 注意事项：
	 * - 必须在使用其他功能前调用
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	void Initialize();

	/**
	 * 清理特殊效果系统
	 * 
	 * 删除所有效果并重置系统状态。
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	void Cleanup();

	// ========== 效果管理 ==========
	
	/**
	 * 创建引力奇点效果
	 * 
	 * @param Params 引力奇点参数
	 * @return 效果ID
	 * 
	 * 注意事项：
	 * - 引力奇点会持续对周围弹珠施加引力
	 * - 如果Duration>0，效果会在指定时间后自动移除
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	FGuid CreateGravityWell(const FGravityWellParams& Params);

	/**
	 * 创建虫洞传送效果
	 * 
	 * @param Params 虫洞参数
	 * @return 效果ID
	 * 
	 * 注意事项：
	 * - 进入虫洞入口半径的弹珠会被传送到出口
	 * - 传送是瞬间的，不会有动画过渡
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	FGuid CreateWormhole(const FWormholeParams& Params);

	/**
	 * 应用分裂效果到指定弹珠
	 * 
	 * @param MarbleState 要分裂的弹珠状态
	 * @param Params 分裂参数
	 * @param OutNewMarbles 输出参数，存储新生成的子弹珠状态
	 * @return 成功分裂的子弹珠数量
	 * 
	 * 注意事项：
	 * - 原弹珠会被标记为待销毁（通过返回值判断）
	 * - 子弹珠的Generation会自动+1
	 * - 需要将新弹珠添加到物理系统
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	int32 ApplySplitEffect(const FMarbleState& MarbleState, const FSplitParams& Params, TArray<FMarbleState>& OutNewMarbles);

	/**
	 * 应用速度修改效果到指定弹珠
	 * 
	 * @param MarbleState 要修改的弹珠状态
	 * @param Params 速度修改参数
	 * @param OutModifiedState 输出参数，存储修改后的弹珠状态
	 * @return true=成功应用，false=失败
	 * 
	 * 注意事项：
	 * - 如果bApplyImmediately=true，速度会立即改变
	 * - 如果Duration>0，效果会持续一段时间
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	bool ApplySpeedModifier(const FMarbleState& MarbleState, const FEchoSpeedModifierParams& Params, FMarbleState& OutModifiedState);

	/**
	 * 应用连锁触发效果
	 * 
	 * @param TriggerPosition 触发位置
	 * @param TriggerDirection 触发方向
	 * @param Params 连锁触发参数
	 * @param OutSecondaryMarbles 输出参数，存储次级弹珠状态
	 * @return 生成的次级弹珠数量
	 * 
	 * 注意事项：
	 * - 次级弹珠会从触发位置向周围扇形区域发射
	 * - 次级弹珠的Generation会比触发弹珠+1
	 * - 需要将次级弹珠添加到物理系统
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	int32 ApplyChainTrigger(FVector TriggerPosition, FVector TriggerDirection, const FChainTriggerParams& Params, TArray<FMarbleState>& OutSecondaryMarbles);

	/**
	 * 移除特殊效果
	 * 
	 * @param EffectID 效果ID
	 * @return true=移除成功，false=ID不存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	bool RemoveEffect(const FGuid& EffectID);

	// ========== 更新 ==========
	
	/**
	 * 更新特殊效果系统
	 * 
	 * @param DeltaTime 时间增量（单位：秒）
	 * @param Marbles 所有弹珠的状态数组
	 * @param OutModifiedMarbles 输出参数，存储被效果修改的弹珠状态
	 * 
	 * 注意事项：
	 * - 必须每帧调用
	 * - 会自动处理引力奇点、虫洞传送等持续性效果
	 * - 会自动移除过期的效果
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	void Tick(float DeltaTime, const TArray<FMarbleState>& Marbles, TArray<FMarbleState>& OutModifiedMarbles);

	// ========== 查询 ==========
	
	/**
	 * 获取所有激活的效果
	 * 
	 * @return 效果数据数组
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffect")
	TArray<FSpecialEffectData> GetAllActiveEffects() const;

	/**
	 * 获取指定类型的效果数量
	 * 
	 * @param EffectType 效果类型
	 * @return 效果数量
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect")
	int32 GetEffectCountByType(EEchoSpecialEffectType EffectType) const;

	/**
	 * 检查系统是否已初始化
	 * 
	 * @return true=已初始化，false=未初始化
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|SpecialEffect")
	bool IsInitialized() const { return bIsInitialized; }

	// ========== 事件 ==========
	
	/** 特殊效果触发事件 */
	UPROPERTY(BlueprintAssignable, Category = "Physics|SpecialEffect|Events")
	FOnSpecialEffectTriggeredDelegate OnEffectTriggered;

private:
	// ========== 内部状态 ==========
	
	/** 是否已初始化 */
	bool bIsInitialized = false;

	/** 当前游戏时间（单位：秒） */
	float CurrentGameTime = 0.0f;

	/** 所有效果的映射表（ID -> Effect） */
	TMap<FGuid, FSpecialEffectData> Effects;

	// ========== 内部辅助函数 ==========
	
	/**
	 * 更新引力奇点效果
	 * 
	 * @param Effect 效果数据
	 * @param Marbles 所有弹珠状态
	 * @param OutModifiedMarbles 输出参数，存储被修改的弹珠状态
	 */
	void UpdateGravityWell(const FSpecialEffectData& Effect, const TArray<FMarbleState>& Marbles, TArray<FMarbleState>& OutModifiedMarbles);

	/**
	 * 更新虫洞传送效果
	 * 
	 * @param Effect 效果数据
	 * @param Marbles 所有弹珠状态
	 * @param OutModifiedMarbles 输出参数，存储被修改的弹珠状态
	 */
	void UpdateWormhole(const FSpecialEffectData& Effect, const TArray<FMarbleState>& Marbles, TArray<FMarbleState>& OutModifiedMarbles);

	/**
	 * 检查并移除过期的效果
	 */
	void RemoveExpiredEffects();
};
