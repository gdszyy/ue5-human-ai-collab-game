// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/DamageCalculator.h"
#include "Misc/DateTime.h"

FDamageInfo UDamageCalculator::CalculateDamage(const FMarbleState& Marble, FGuid TargetID)
{
	FDamageInfo DamageInfo;
	
	// 设置伤害来源和目标
	DamageInfo.SourceID = Marble.ID;
	DamageInfo.PotionType = Marble.PotionType;
	DamageInfo.TargetID = TargetID;
	DamageInfo.HitCount = Marble.HitCount;
	DamageInfo.PotencyRemaining = Marble.Potency;
	
	// 基础伤害
	DamageInfo.BaseDamage = Marble.BaseDamage;
	
	// 伤害加成（仅弹射药剂）
	if (Marble.PotionType == EPotionType::Ricochet)
	{
		DamageInfo.DamageBonus = CalculateRicochetDamageBonus(Marble.HitCount);
	}
	else
	{
		DamageInfo.DamageBonus = 0.0f;
	}
	
	// 药效强度削减
	const float PotencyRequired = GetPotencyRequired(Marble.PotionType);
	DamageInfo.PotencyRequired = PotencyRequired;
	DamageInfo.PotencyReduction = CalculatePotencyReduction(Marble.Potency, PotencyRequired);
	
	// 最终伤害 = (基础伤害 + 伤害加成) × 药效削减系数
	DamageInfo.FinalDamage = (DamageInfo.BaseDamage + DamageInfo.DamageBonus) * DamageInfo.PotencyReduction;
	
	// 设置时间戳
	DamageInfo.Timestamp = FPlatformTime::Seconds();
	
	return DamageInfo;
}

float UDamageCalculator::CalculateRicochetDamageBonus(int32 HitCount)
{
	if (HitCount <= 0)
	{
		return 0.0f;
	}
	
	// 计算当前里程碑
	// 伤害加成基于自然数和序列 Sn = 1+2+...+n
	// 当撞击次数达到 Sk 时，伤害加成增加到 +k
	int32 k = 0;
	int32 Sk = 0;
	
	while (true)
	{
		k++;
		Sk += k; // Sk = 1 + 2 + ... + k
		
		if (HitCount < Sk)
		{
			return static_cast<float>(k - 1);
		}
		
		// 安全检查：防止无限循环
		if (k > 1000)
		{
			UE_LOG(LogTemp, Warning, TEXT("CalculateRicochetDamageBonus: HitCount too large (%d), capping at k=1000"), HitCount);
			return static_cast<float>(k - 1);
		}
	}
}

float UDamageCalculator::CalculatePotencyReduction(float PotencyRemaining, float PotencyRequired)
{
	if (PotencyRequired <= 0.0f)
	{
		// 如果需求为0，返回1.0（无削减）
		return 1.0f;
	}
	
	if (PotencyRemaining >= PotencyRequired)
	{
		// 药效充足，无削减
		return 1.0f;
	}
	else if (PotencyRemaining > 0.0f)
	{
		// 药效不足，按比例削减
		return FMath::Clamp(PotencyRemaining / PotencyRequired, 0.0f, 1.0f);
	}
	else
	{
		// 药效耗尽，无伤害
		return 0.0f;
	}
}

float UDamageCalculator::GetPotencyRequired(EPotionType PotionType)
{
	switch (PotionType)
	{
	case EPotionType::Ricochet:
		return 1.0f;
	case EPotionType::Piercing:
		return 1.5f;
	case EPotionType::Explosive:
		return 3.0f;
	default:
		return 1.0f;
	}
}

float UDamageCalculator::CalculateCriticalDamage(
	float BaseDamage,
	float CriticalChance,
	float CriticalMultiplier,
	bool& OutIsCritical
)
{
	// 生成随机数判断是否暴击
	const float RandomValue = FMath::FRand();
	OutIsCritical = RandomValue < CriticalChance;
	
	if (OutIsCritical)
	{
		return BaseDamage * CriticalMultiplier;
	}
	else
	{
		return BaseDamage;
	}
}

float UDamageCalculator::CalculateArmorReduction(float Damage, float Armor)
{
	// 简单的护甲减伤公式：减伤率 = Armor / (Armor + 100)
	// 例如：100点护甲 = 50%减伤，200点护甲 = 66.7%减伤
	if (Armor <= 0.0f)
	{
		return Damage;
	}
	
	const float ReductionRate = Armor / (Armor + 100.0f);
	const float ReducedDamage = Damage * (1.0f - ReductionRate);
	
	return FMath::Max(ReducedDamage, 0.0f);
}

void UDamageCalculator::PrintDamageInfo(const FDamageInfo& DamageInfo)
{
	UE_LOG(LogTemp, Log, TEXT("=== Damage Info ==="));
	UE_LOG(LogTemp, Log, TEXT("Source ID: %s"), *DamageInfo.SourceID.ToString());
	UE_LOG(LogTemp, Log, TEXT("Target ID: %s"), *DamageInfo.TargetID.ToString());
	UE_LOG(LogTemp, Log, TEXT("Potion Type: %s"), *DamageInfo.GetPotionTypeName());
	UE_LOG(LogTemp, Log, TEXT("Base Damage: %.1f"), DamageInfo.BaseDamage);
	UE_LOG(LogTemp, Log, TEXT("Damage Bonus: %.1f"), DamageInfo.DamageBonus);
	UE_LOG(LogTemp, Log, TEXT("Potency Reduction: %.2f"), DamageInfo.PotencyReduction);
	UE_LOG(LogTemp, Log, TEXT("Final Damage: %.1f"), DamageInfo.FinalDamage);
	UE_LOG(LogTemp, Log, TEXT("Hit Count: %d"), DamageInfo.HitCount);
	UE_LOG(LogTemp, Log, TEXT("Potency: %.1f / %.1f"), DamageInfo.PotencyRemaining, DamageInfo.PotencyRequired);
	UE_LOG(LogTemp, Log, TEXT("Is Lethal: %s"), DamageInfo.bIsLethal ? TEXT("Yes") : TEXT("No"));
	UE_LOG(LogTemp, Log, TEXT("Is Critical: %s"), DamageInfo.bIsCritical ? TEXT("Yes") : TEXT("No"));
	UE_LOG(LogTemp, Log, TEXT("=================="));
}

TArray<FString> UDamageCalculator::GenerateDamageBonusTable(int32 MaxHitCount)
{
	TArray<FString> Table;
	
	// 表头
	Table.Add(TEXT("=== Ricochet Potion Damage Bonus Table ==="));
	Table.Add(TEXT("Hit Count | Milestone | Damage Bonus | Example Damage (Base: 10)"));
	Table.Add(TEXT("----------|-----------|--------------|---------------------------"));
	
	// 生成表格内容
	int32 CurrentBonus = 0;
	int32 k = 1;
	int32 Sk = 1;
	
	for (int32 HitCount = 1; HitCount <= MaxHitCount; HitCount++)
	{
		// 检查是否达到里程碑
		bool bIsMilestone = (HitCount == Sk);
		if (bIsMilestone)
		{
			CurrentBonus = k;
			k++;
			Sk += k;
		}
		
		// 计算示例伤害
		const float BaseDamage = 10.0f;
		const float ExampleDamage = BaseDamage + CurrentBonus;
		
		// 格式化输出
		FString MilestoneStr = bIsMilestone ? FString::Printf(TEXT("S%d=%d ✓"), CurrentBonus, HitCount) : TEXT("-");
		FString Row = FString::Printf(
			TEXT("%9d | %9s | %12d | %.1f"),
			HitCount,
			*MilestoneStr,
			CurrentBonus,
			ExampleDamage
		);
		
		Table.Add(Row);
	}
	
	Table.Add(TEXT("=========================================="));
	
	return Table;
}

int32 UDamageCalculator::CalculateNaturalSum(int32 k)
{
	if (k <= 0)
	{
		return 0;
	}
	return k * (k + 1) / 2;
}
