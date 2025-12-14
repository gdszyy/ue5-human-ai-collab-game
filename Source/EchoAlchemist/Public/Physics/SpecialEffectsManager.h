// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Physics/SpecialEffectData.h"
#include "Physics/MarbleState.h"
#include "SpecialEffectsManager.generated.h"

/**
 * 特殊效果管理器
 * 
 * 负责管理和应用所有特殊物理效果，包括引力场、传送、分裂等。
 * 
 * 蓝图使用示例：
 * 
 * 1. 创建引力奇点
 *    ```
 *    FGravityWellParams Params;
 *    Params.Position = GetActorLocation();
 *    Params.Strength = 500.0f;
 *    Params.Radius = 300.0f;
 *    
 *    FGuid SingularityID = EffectsManager->CreateGravitySingularity(Params);
 *    ```
 * 
 * 2. 创建虫洞
 *    ```
 *    FWormholeParams Params;
 *    Params.EntryPosition = EntryLocation;
 *    Params.ExitPosition = ExitLocation;
 *    Params.EntryRadius = 50.0f;
 *    
 *    FGuid WormholeID = EffectsManager->CreateWormhole(Params);
 *    ```
 * 
 * 3. 应用效果到魔力露珠
 *    ```
 *    // 每帧调用，应用所有活跃的效果
 *    EffectsManager->ApplyEffects(Marbles, DeltaTime);
 *    ```
 * 
 * 注意事项：
 * - 必须每帧调用ApplyEffects来应用效果
 * - 效果会自动过期并清理
 * - 分裂和连锁效果会返回新生成的魔力露珠
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API USpecialEffectsManager : public UObject
{
	GENERATED_BODY()

public:
	// ========== 引力奇点 ==========
	
	/**
	 * 创建引力奇点
	 * 
	 * @param Params 引力奇点参数
	 * @return 奇点的唯一ID
	 * 
	 * 注意事项：
	 * - 引力强度建议范围：100-1000 cm/s²
	 * - 影响半径建议范围：100-500 cm
	 * - Duration=0表示永久存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Gravity")
	FGuid CreateGravitySingularity(const FGravityWellParams& Params);

	/**
	 * 移除引力奇点
	 * 
	 * @param SingularityID 奇点ID
	 * @return true=移除成功，false=ID不存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Gravity")
	bool RemoveGravitySingularity(const FGuid& SingularityID);

	/**
	 * 获取所有活跃的引力奇点
	 * 
	 * @return 所有引力奇点的参数数组
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Gravity")
	TArray<FGravityWellParams> GetAllGravitySingularities() const;

	// ========== 虫洞传送 ==========
	
	/**
	 * 创建虫洞
	 * 
	 * @param Params 虫洞参数
	 * @return 虫洞的唯一ID
	 * 
	 * 注意事项：
	 * - 入口半径建议范围：30-100 cm
	 * - 可以创建多个虫洞形成传送网络
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Wormhole")
	FGuid CreateWormhole(const FWormholeParams& Params);

	/**
	 * 移除虫洞
	 * 
	 * @param WormholeID 虫洞ID
	 * @return true=移除成功，false=ID不存在
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Wormhole")
	bool RemoveWormhole(const FGuid& WormholeID);

	/**
	 * 获取所有活跃的虫洞
	 * 
	 * @return 所有虫洞的参数数组
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Wormhole")
	TArray<FWormholeParams> GetAllWormholes() const;

	// ========== 效果应用 ==========
	
	/**
	 * 应用所有效果到魔力露珠
	 * 
	 * @param Marbles 魔力露珠状态数组（引用，会被修改）
	 * @param DeltaTime 时间增量
	 * 
	 * 注意事项：
	 * - 必须每帧调用
	 * - 会自动清理过期的效果
	 * - 引力场会修改魔力露珠的速度
	 * - 虫洞会修改魔力露珠的位置
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects")
	void ApplyEffects(UPARAM(ref) TArray<FMarbleState>& Marbles, float DeltaTime);

	/**
	 * 应用弹珠分裂效果
	 * 
	 * @param ParentMarble 父代魔力露珠状态
	 * @param Params 分裂参数
	 * @param OutChildMarbles 输出参数，存储生成的子代魔力露珠
	 * @return 生成的子代魔力露珠数量
	 * 
	 * 使用场景：
	 * - 魔力露珠碰撞时调用
	 * - 特定魔药效果触发时调用
	 * 
	 * 注意事项：
	 * - 子代魔力露珠的代数会+1
	 * - 子代魔力露珠的药效强度会按比例分配
	 * - 会检查最大分裂深度，防止无限分裂
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Split")
	int32 ApplyMarbleSplit(const FMarbleState& ParentMarble, const FSplitParams& Params, 
	                       TArray<FMarbleState>& OutChildMarbles);

	/**
	 * 应用速度修改效果
	 * 
	 * @param Marble 魔力露珠状态（引用，会被修改）
	 * @param Params 速度修改参数
	 * 
	 * 使用场景：
	 * - 魔力露珠进入加速/减速区域
	 * - 特定魔药效果触发
	 * 
	 * 注意事项：
	 * - 立即应用速度变化
	 * - 如果bSmoothTransition=true，需要在多帧中调用
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Speed")
	void ApplySpeedModifier(UPARAM(ref) FMarbleState& Marble, const FEchoSpeedModifierParams& Params);

	/**
	 * 应用连锁触发效果
	 * 
	 * @param TriggerMarble 触发魔力露珠状态
	 * @param Params 连锁触发参数
	 * @param OutProjectiles 输出参数，存储生成的次级魔药
	 * @return 生成的次级魔药数量
	 * 
	 * 使用场景：
	 * - 魔力露珠碰撞敌人时调用
	 * - 特定魔药效果触发时调用
	 * 
	 * 注意事项：
	 * - 次级魔药会以扇形方式发射
	 * - 次级魔药的代数会+1
	 * - 会检查最大连锁深度，防止无限连锁
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Chain")
	int32 ApplyChainReaction(const FMarbleState& TriggerMarble, const FChainTriggerParams& Params, 
	                         TArray<FMarbleState>& OutProjectiles);

	// ========== 系统管理 ==========
	
	/**
	 * 更新效果系统
	 * 
	 * @param DeltaTime 时间增量
	 * 
	 * 注意事项：
	 * - 每帧调用一次
	 * - 会清理过期的效果
	 * - 会更新游戏时间
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects")
	void Tick(float DeltaTime);

	/**
	 * 清理所有效果
	 * 
	 * 删除所有引力奇点、虫洞等效果。
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects")
	void ClearAllEffects();

	/**
	 * 获取效果统计信息
	 * 
	 * @param OutGravityCount 引力奇点数量
	 * @param OutWormholeCount 虫洞数量
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|SpecialEffects|Debug")
	void GetStatistics(int32& OutGravityCount, int32& OutWormholeCount) const;

private:
	// ========== 内部状态 ==========
	
	/** 活跃的引力奇点映射表（ID -> Params） */
	TMap<FGuid, FGravityWellParams> GravitySingularities;

	/** 活跃的虫洞映射表（ID -> Params） */
	TMap<FGuid, FWormholeParams> Wormholes;

	/** 当前游戏时间（单位：秒） */
	float CurrentGameTime = 0.0f;

	// ========== 内部辅助函数 ==========
	
	/**
	 * 应用引力场效果
	 * 
	 * @param Marble 魔力露珠状态（引用，会被修改）
	 * @param DeltaTime 时间增量
	 */
	void ApplyGravityFields(FMarbleState& Marble, float DeltaTime);

	/**
	 * 应用虫洞传送效果
	 * 
	 * @param Marble 魔力露珠状态（引用，会被修改）
	 */
	void ApplyWormholes(FMarbleState& Marble);

	/**
	 * 清理过期的效果
	 */
	void CleanupExpiredEffects();

	/**
	 * 计算分裂方向
	 * 
	 * @param ParentVelocity 父代速度
	 * @param SplitCount 分裂数量
	 * @param AngleSpread 角度扩散
	 * @param OutDirections 输出参数，存储分裂方向
	 */
	void CalculateSplitDirections(const FVector& ParentVelocity, int32 SplitCount, 
	                               float AngleSpread, TArray<FVector>& OutDirections);

	/**
	 * 计算连锁发射方向
	 * 
	 * @param TriggerVelocity 触发魔药速度
	 * @param ProjectileCount 次级魔药数量
	 * @param AngleSpread 角度扩散
	 * @param OutDirections 输出参数，存储发射方向
	 */
	void CalculateChainDirections(const FVector& TriggerVelocity, int32 ProjectileCount, 
	                               float AngleSpread, TArray<FVector>& OutDirections);
};
