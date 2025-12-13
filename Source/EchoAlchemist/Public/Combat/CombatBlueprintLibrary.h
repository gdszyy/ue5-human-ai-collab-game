// Copyright Echo Alchemist Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatConfig.h"
#include "Physics/MarbleState.h"
#include "EnemyData.h"
#include "DamageInfo.h"
#include "Physics/PhysicsSceneConfig.h"
#include "CombatBlueprintLibrary.generated.h"

/**
 * 战斗系统蓝图函数库
 * 为蓝图提供便捷的战斗系统接口
 * 
 * 功能分类：
 * 1. 配置创建：创建各种配置对象
 * 2. 状态查询：查询战斗状态
 * 3. 数据转换：数据格式转换
 * 4. 工具函数：常用的辅助函数
 */
UCLASS()
class ECHOALCHEMIST_API UCombatBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ========== 配置创建 ==========
	
	/**
	 * 创建战斗配置
	 * @param Difficulty 难度等级（1=简单，5=普通，10=困难）
	 * @return 战斗配置
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Config", meta = (Keywords = "create config"))
	static FCombatConfig CreateCombatConfig(int32 Difficulty = 5);

	/**
	 * 创建物理场景配置
	 * @param SceneType 场景类型
	 * @return 物理场景配置
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Config", meta = (Keywords = "create config"))
	static FPhysicsSceneConfig CreatePhysicsSceneConfig(EPhysicsSceneType SceneType);

	// ========== 魔力露珠/魔药 ==========
	
	/**
	 * 创建魔力露珠/魔药状态
	 * @param PotionType 魔药类型
	 * @param Position 位置
	 * @param Velocity 速度
	 * @param BaseDamage 基础伤害
	 * @param Potency 药效强度
	 * @return 魔力露珠/魔药状态
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Marble", meta = (Keywords = "create marble"))
	static FMarbleState CreateMarbleState(
		EPotionType PotionType,
		FVector Position,
		FVector Velocity,
		float BaseDamage,
		float Potency
	);

	/**
	 * 检查魔力露珠/魔药是否有效
	 * @param Marble 魔力露珠/魔药状态
	 * @return 是否有效
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Marble", meta = (Keywords = "check valid"))
	static bool IsMarbleValid(const FMarbleState& Marble);

	// ========== 敌人 ==========
	
	/**
	 * 创建敌人数据
	 * @param EnemyType 敌人类型
	 * @param Position 位置
	 * @param MaxHealth 最大生命值
	 * @return 敌人数据
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Enemy", meta = (Keywords = "create enemy"))
	static FEnemyData CreateEnemyData(
		EEnemyType EnemyType,
		FVector Position,
		float MaxHealth
	);

	/**
	 * 检查敌人是否存活
	 * @param Enemy 敌人数据
	 * @return 是否存活
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Enemy", meta = (Keywords = "check alive"))
	static bool IsEnemyAlive(const FEnemyData& Enemy);

	/**
	 * 应用伤害到敌人
	 * @param Enemy 敌人数据（输入/输出）
	 * @param Damage 伤害值
	 * @return 是否死亡
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Enemy", meta = (Keywords = "apply damage"))
	static bool ApplyDamageToEnemy(UPARAM(ref) FEnemyData& Enemy, float Damage);

	// ========== 伤害计算 ==========
	
	/**
	 * 计算伤害
	 * @param Marble 魔力露珠/魔药状态
	 * @param TargetID 目标敌人ID
	 * @return 伤害信息
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Damage", meta = (Keywords = "calculate damage"))
	static FDamageInfo CalculateDamage(const FMarbleState& Marble, FGuid TargetID);

	/**
	 * 获取伤害描述字符串
	 * @param DamageInfo 伤害信息
	 * @return 伤害描述字符串
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Damage", meta = (Keywords = "damage string"))
	static FString GetDamageInfoString(const FDamageInfo& DamageInfo);

	// ========== 工具函数 ==========
	
	/**
	 * 生成新的GUID
	 * @return 新的GUID
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Utility", meta = (Keywords = "generate guid"))
	static FGuid GenerateGuid();

	/**
	 * 检查两个GUID是否相等
	 * @param A GUID A
	 * @param B GUID B
	 * @return 是否相等
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Utility", meta = (Keywords = "compare guid"))
	static bool IsGuidEqual(FGuid A, FGuid B);

	/**
	 * GUID转字符串
	 * @param Guid GUID
	 * @return 字符串
	 */
	UFUNCTION(BlueprintPure, Category = "Combat|Utility", meta = (Keywords = "guid string"))
	static FString GuidToString(FGuid Guid);
};
