// Copyright 2025 Voidzyy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Physics/MarbleActor.h"
#include "MarbleActorPool.generated.h"

/**
 * 魔力露珠Actor对象池
 * 
 * 管理AMarbleActor的对象池，复用Actor对象以提升性能。
 * 避免频繁创建和销毁Actor带来的性能开销。
 * 
 * 蓝图使用示例：
 * 
 * 1. 创建对象池
 *    ```
 *    MarblePool = NewObject<UMarbleActorPool>();
 *    MarblePool->Initialize(GetWorld(), 50);  // 预创建50个Actor
 *    ```
 * 
 * 2. 从对象池获取Actor
 *    ```
 *    AMarbleActor* Marble = MarblePool->Acquire();
 *    if (Marble)
 *    {
 *        // 初始化并使用
 *        Marble->InitializeFromState(State);
 *    }
 *    ```
 * 
 * 3. 归还Actor到对象池
 *    ```
 *    MarblePool->Release(Marble);
 *    ```
 * 
 * 4. 清空对象池
 *    ```
 *    MarblePool->Clear();
 *    ```
 * 
 * 性能优化：
 * - 预创建：在初始化时预创建一定数量的Actor
 * - 复用：归还的Actor会被重置并放回池中
 * - 扩展：池中Actor不足时自动创建新的
 * - 统计：提供使用统计信息用于性能分析
 * 
 * 注意事项：
 * - 必须先调用Initialize才能使用
 * - 归还的Actor会自动重置状态
 * - 对象池不会自动销毁Actor，需要手动调用Clear
 */
UCLASS(BlueprintType)
class ECHOALCHEMIST_API UMarbleActorPool : public UObject
{
	GENERATED_BODY()

public:
	// ========== 初始化和清理 ==========
	
	/**
	 * 初始化对象池
	 * 
	 * @param World 世界对象（用于生成Actor）
	 * @param PreAllocateCount 预创建的Actor数量
	 * 
	 * 注意事项：
	 * - 必须在使用其他功能前调用
	 * - 预创建的Actor会被隐藏并放入池中
	 * - 预创建数量建议根据场景需求设置（炼金工作台：10，战斗场景：50）
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Pool")
	void Initialize(UWorld* World, int32 PreAllocateCount = 20);

	/**
	 * 清空对象池
	 * 
	 * 销毁所有Actor并重置对象池。
	 * 
	 * 注意事项：
	 * - 会销毁所有Actor，包括正在使用的
	 * - 通常在切换关卡或结束游戏时调用
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Pool")
	void Clear();

	// ========== 对象池操作 ==========
	
	/**
	 * 从对象池获取Actor
	 * 
	 * @return Actor对象，如果池为空则创建新的
	 * 
	 * 注意事项：
	 * - 返回的Actor处于激活状态
	 * - 如果池为空，会自动创建新的Actor
	 * - 使用完毕后必须调用Release归还
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Pool")
	AMarbleActor* Acquire();

	/**
	 * 归还Actor到对象池
	 * 
	 * @param Actor 要归还的Actor
	 * 
	 * 注意事项：
	 * - Actor会被自动重置状态
	 * - Actor会被隐藏并禁用碰撞
	 * - 不要在归还后继续使用Actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Pool")
	void Release(AMarbleActor* Actor);

	// ========== 统计信息 ==========
	
	/**
	 * 获取对象池统计信息
	 * 
	 * @param OutTotalCount 总Actor数量
	 * @param OutAvailableCount 可用Actor数量
	 * @param OutInUseCount 正在使用的Actor数量
	 * 
	 * 使用场景：
	 * - 性能分析
	 * - 调试对象池使用情况
	 * - 动态调整预创建数量
	 */
	UFUNCTION(BlueprintCallable, Category = "Physics|Pool|Debug")
	void GetStatistics(int32& OutTotalCount, int32& OutAvailableCount, int32& OutInUseCount) const;

	/**
	 * 获取可用Actor数量
	 * 
	 * @return 池中可用的Actor数量
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Pool")
	int32 GetAvailableCount() const { return AvailableActors.Num(); }

	/**
	 * 获取正在使用的Actor数量
	 * 
	 * @return 正在使用的Actor数量
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Pool")
	int32 GetInUseCount() const { return InUseActors.Num(); }

	/**
	 * 检查对象池是否已初始化
	 * 
	 * @return true=已初始化，false=未初始化
	 */
	UFUNCTION(BlueprintPure, Category = "Physics|Pool")
	bool IsInitialized() const { return bIsInitialized; }

private:
	// ========== 内部状态 ==========
	
	/** 是否已初始化 */
	bool bIsInitialized = false;

	/** 世界对象 */
	UPROPERTY()
	UWorld* CachedWorld = nullptr;

	/** 可用的Actor列表 */
	UPROPERTY()
	TArray<AMarbleActor*> AvailableActors;

	/** 正在使用的Actor列表 */
	UPROPERTY()
	TArray<AMarbleActor*> InUseActors;

	// ========== 内部辅助函数 ==========
	
	/**
	 * 创建新的Actor
	 * 
	 * @return 新创建的Actor
	 */
	AMarbleActor* CreateNewActor();
};
