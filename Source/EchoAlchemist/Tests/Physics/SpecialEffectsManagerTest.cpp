// Copyright 2025 Voidzyy. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Physics/SpecialEffectsManager.h"
#include "Physics/MarbleState.h"

// 测试：创建引力奇点
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectsGravitySingularityTest, 
	"EchoAlchemist.Physics.SpecialEffects.GravitySingularity", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectsGravitySingularityTest::RunTest(const FString& Parameters)
{
	// 创建特殊效果管理器
	USpecialEffectsManager* EffectsManager = NewObject<USpecialEffectsManager>();
	TestNotNull(TEXT("EffectsManager should be created"), EffectsManager);

	// 创建引力奇点
	FGravitySingularityParams Params;
	Params.Position = FVector(0, 0, 0);
	Params.Strength = 500.0f;
	Params.Radius = 300.0f;
	Params.Duration = 0.0f;  // 永久

	FGuid SingularityID = EffectsManager->CreateGravitySingularity(Params);

	// 验证创建结果
	TestTrue(TEXT("SingularityID should be valid"), SingularityID.IsValid());

	// 获取所有引力奇点
	TArray<FGravitySingularityParams> Singularities = EffectsManager->GetAllGravitySingularities();
	TestEqual(TEXT("Should have 1 singularity"), Singularities.Num(), 1);

	// 创建魔力露珠
	FMarbleState Marble;
	Marble.Position = FVector(200, 0, 0);  // 距离奇点200cm
	Marble.Velocity = FVector(0, 0, 0);    // 初始静止

	// 应用引力效果（1秒）
	TArray<FMarbleState> Marbles;
	Marbles.Add(Marble);
	EffectsManager->ApplyEffects(Marbles, 1.0f);

	// 验证魔力露珠被吸引（速度应该指向奇点）
	FVector VelocityDirection = Marbles[0].Velocity.GetSafeNormal();
	FVector ExpectedDirection = (Params.Position - Marble.Position).GetSafeNormal();
	float DotProduct = FVector::DotProduct(VelocityDirection, ExpectedDirection);
	TestTrue(TEXT("Marble should be attracted to singularity"), DotProduct > 0.9f);

	// 移除引力奇点
	bool bRemoved = EffectsManager->RemoveGravitySingularity(SingularityID);
	TestTrue(TEXT("Singularity should be removed"), bRemoved);

	return true;
}

// 测试：虫洞传送
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectsWormholeTest, 
	"EchoAlchemist.Physics.SpecialEffects.Wormhole", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectsWormholeTest::RunTest(const FString& Parameters)
{
	// 创建特殊效果管理器
	USpecialEffectsManager* EffectsManager = NewObject<USpecialEffectsManager>();

	// 创建虫洞
	FWormholeTeleportParams Params;
	Params.EntryPosition = FVector(0, 0, 0);
	Params.ExitPosition = FVector(1000, 0, 0);
	Params.EntryRadius = 50.0f;
	Params.bPreserveVelocity = true;
	Params.VelocityMultiplier = 1.0f;

	FGuid WormholeID = EffectsManager->CreateWormhole(Params);

	// 验证创建结果
	TestTrue(TEXT("WormholeID should be valid"), WormholeID.IsValid());

	// 创建魔力露珠（在入口附近）
	FMarbleState Marble;
	Marble.Position = FVector(10, 0, 0);  // 距离入口10cm
	Marble.Velocity = FVector(100, 0, 0);

	// 应用虫洞效果
	TArray<FMarbleState> Marbles;
	Marbles.Add(Marble);
	EffectsManager->ApplyEffects(Marbles, 0.1f);

	// 验证魔力露珠被传送到出口
	TestEqual(TEXT("Marble should be teleported to exit"), Marbles[0].Position, Params.ExitPosition);

	// 移除虫洞
	bool bRemoved = EffectsManager->RemoveWormhole(WormholeID);
	TestTrue(TEXT("Wormhole should be removed"), bRemoved);

	return true;
}

// 测试：弹珠分裂
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectsMarbleSplitTest, 
	"EchoAlchemist.Physics.SpecialEffects.MarbleSplit", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectsMarbleSplitTest::RunTest(const FString& Parameters)
{
	// 创建特殊效果管理器
	USpecialEffectsManager* EffectsManager = NewObject<USpecialEffectsManager>();

	// 创建父代魔力露珠
	FMarbleState ParentMarble;
	ParentMarble.Position = FVector(0, 0, 0);
	ParentMarble.Velocity = FVector(1000, 0, 0);
	ParentMarble.Potency = 5.0f;
	ParentMarble.Generation = 0;

	// 创建分裂参数
	FMarbleSplitParams Params;
	Params.SplitCount = 3;
	Params.SpeedMultiplier = 0.8f;
	Params.AngleSpread = 60.0f;
	Params.PotencyRatio = 0.5f;

	// 应用分裂效果
	TArray<FMarbleState> ChildMarbles;
	int32 Count = EffectsManager->ApplyMarbleSplit(ParentMarble, Params, ChildMarbles);

	// 验证分裂结果
	TestEqual(TEXT("Should create 3 child marbles"), Count, 3);
	TestEqual(TEXT("ChildMarbles array should have 3 elements"), ChildMarbles.Num(), 3);

	// 验证子代属性
	for (const FMarbleState& Child : ChildMarbles)
	{
		// 验证位置（应该在父代位置）
		TestEqual(TEXT("Child position should match parent"), Child.Position, ParentMarble.Position);

		// 验证代数（应该+1）
		TestEqual(TEXT("Child generation should be parent+1"), Child.Generation, ParentMarble.Generation + 1);

		// 验证药效强度（应该是父代的一半）
		TestEqual(TEXT("Child potency should be half of parent"), Child.Potency, ParentMarble.Potency * Params.PotencyRatio);

		// 验证速度大小（应该是父代的80%）
		float ChildSpeed = Child.Velocity.Size();
		float ExpectedSpeed = ParentMarble.Velocity.Size() * Params.SpeedMultiplier;
		TestTrue(TEXT("Child speed should be 80% of parent"), FMath::IsNearlyEqual(ChildSpeed, ExpectedSpeed, 1.0f));
	}

	return true;
}

// 测试：速度修改
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectsSpeedModifierTest, 
	"EchoAlchemist.Physics.SpecialEffects.SpeedModifier", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectsSpeedModifierTest::RunTest(const FString& Parameters)
{
	// 创建特殊效果管理器
	USpecialEffectsManager* EffectsManager = NewObject<USpecialEffectsManager>();

	// 创建魔力露珠
	FMarbleState Marble;
	Marble.Velocity = FVector(1000, 0, 0);

	// 记录初始速度
	float InitialSpeed = Marble.Velocity.Size();

	// 创建加速参数
	FEchoSpeedModifierParams BoostParams;
	BoostParams.Multiplier = 1.5f;  // 加速50%

	// 应用加速效果
	EffectsManager->ApplySpeedModifier(Marble, BoostParams);

	// 验证速度增加
	float BoostedSpeed = Marble.Velocity.Size();
	float ExpectedSpeed = InitialSpeed * BoostParams.Multiplier;
	TestTrue(TEXT("Speed should be boosted by 50%"), FMath::IsNearlyEqual(BoostedSpeed, ExpectedSpeed, 1.0f));

	// 创建减速参数
	FEchoSpeedModifierParams SlowParams;
	SlowParams.Multiplier = 0.5f;  // 减速50%

	// 应用减速效果
	EffectsManager->ApplySpeedModifier(Marble, SlowParams);

	// 验证速度降低
	float SlowedSpeed = Marble.Velocity.Size();
	ExpectedSpeed = BoostedSpeed * SlowParams.Multiplier;
	TestTrue(TEXT("Speed should be slowed by 50%"), FMath::IsNearlyEqual(SlowedSpeed, ExpectedSpeed, 1.0f));

	return true;
}

// 测试：连锁触发
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectsChainReactionTest, 
	"EchoAlchemist.Physics.SpecialEffects.ChainReaction", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectsChainReactionTest::RunTest(const FString& Parameters)
{
	// 创建特殊效果管理器
	USpecialEffectsManager* EffectsManager = NewObject<USpecialEffectsManager>();

	// 创建触发魔力露珠
	FMarbleState TriggerMarble;
	TriggerMarble.Position = FVector(0, 0, 0);
	TriggerMarble.Velocity = FVector(1000, 0, 0);
	TriggerMarble.BaseDamage = 10.0f;
	TriggerMarble.Generation = 0;

	// 创建连锁触发参数
	FChainReactionParams Params;
	Params.ProjectileCount = 4;
	Params.ProjectileSpeed = 800.0f;
	Params.AngleSpread = 90.0f;
	Params.Potency = 2.0f;
	Params.DamageMultiplier = 0.5f;

	// 应用连锁触发效果
	TArray<FMarbleState> Projectiles;
	int32 Count = EffectsManager->ApplyChainReaction(TriggerMarble, Params, Projectiles);

	// 验证连锁触发结果
	TestEqual(TEXT("Should create 4 projectiles"), Count, 4);
	TestEqual(TEXT("Projectiles array should have 4 elements"), Projectiles.Num(), 4);

	// 验证次级魔药属性
	for (const FMarbleState& Projectile : Projectiles)
	{
		// 验证位置（应该在触发魔药位置）
		TestEqual(TEXT("Projectile position should match trigger"), Projectile.Position, TriggerMarble.Position);

		// 验证代数（应该+1）
		TestEqual(TEXT("Projectile generation should be trigger+1"), Projectile.Generation, TriggerMarble.Generation + 1);

		// 验证药效强度
		TestEqual(TEXT("Projectile potency should match params"), Projectile.Potency, Params.Potency);

		// 验证伤害倍率
		float ExpectedDamage = TriggerMarble.BaseDamage * Params.DamageMultiplier;
		TestEqual(TEXT("Projectile damage should be half of trigger"), Projectile.BaseDamage, ExpectedDamage);

		// 验证速度大小
		float ProjectileSpeed = Projectile.Velocity.Size();
		TestTrue(TEXT("Projectile speed should match params"), FMath::IsNearlyEqual(ProjectileSpeed, Params.ProjectileSpeed, 1.0f));
	}

	return true;
}

// 测试：效果过期清理
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectsExpirationTest, 
	"EchoAlchemist.Physics.SpecialEffects.Expiration", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectsExpirationTest::RunTest(const FString& Parameters)
{
	// 创建特殊效果管理器
	USpecialEffectsManager* EffectsManager = NewObject<USpecialEffectsManager>();

	// 创建临时引力奇点（持续3秒）
	FGravitySingularityParams SingularityParams;
	SingularityParams.Position = FVector(0, 0, 0);
	SingularityParams.Strength = 500.0f;
	SingularityParams.Radius = 300.0f;
	SingularityParams.Duration = 3.0f;

	FGuid SingularityID = EffectsManager->CreateGravitySingularity(SingularityParams);

	// 创建临时虫洞（持续2秒）
	FWormholeTeleportParams WormholeParams;
	WormholeParams.EntryPosition = FVector(0, 0, 0);
	WormholeParams.ExitPosition = FVector(1000, 0, 0);
	WormholeParams.EntryRadius = 50.0f;
	WormholeParams.Duration = 2.0f;

	FGuid WormholeID = EffectsManager->CreateWormhole(WormholeParams);

	// 验证效果已创建
	int32 GravityCount, WormholeCount;
	EffectsManager->GetStatistics(GravityCount, WormholeCount);
	TestEqual(TEXT("Should have 1 gravity singularity"), GravityCount, 1);
	TestEqual(TEXT("Should have 1 wormhole"), WormholeCount, 1);

	// 模拟时间流逝（2.5秒）
	EffectsManager->Tick(2.5f);

	// 验证虫洞已过期，引力奇点仍存在
	EffectsManager->GetStatistics(GravityCount, WormholeCount);
	TestEqual(TEXT("Gravity singularity should still exist"), GravityCount, 1);
	TestEqual(TEXT("Wormhole should be expired"), WormholeCount, 0);

	// 模拟时间流逝（再过1秒，总共3.5秒）
	EffectsManager->Tick(1.0f);

	// 验证所有效果都已过期
	EffectsManager->GetStatistics(GravityCount, WormholeCount);
	TestEqual(TEXT("All effects should be expired"), GravityCount, 0);
	TestEqual(TEXT("All effects should be expired"), WormholeCount, 0);

	return true;
}

// 测试：最大分裂深度限制
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectsMaxSplitDepthTest, 
	"EchoAlchemist.Physics.SpecialEffects.MaxSplitDepth", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectsMaxSplitDepthTest::RunTest(const FString& Parameters)
{
	// 创建特殊效果管理器
	USpecialEffectsManager* EffectsManager = NewObject<USpecialEffectsManager>();

	// 创建父代魔力露珠（代数已达到最大深度）
	FMarbleState ParentMarble;
	ParentMarble.Position = FVector(0, 0, 0);
	ParentMarble.Velocity = FVector(1000, 0, 0);
	ParentMarble.Potency = 5.0f;
	ParentMarble.Generation = 2;  // 已达到最大深度

	// 创建分裂参数（最大深度为2）
	FMarbleSplitParams Params;
	Params.SplitCount = 3;
	Params.MaxSplitDepth = 2;

	// 尝试分裂
	TArray<FMarbleState> ChildMarbles;
	int32 Count = EffectsManager->ApplyMarbleSplit(ParentMarble, Params, ChildMarbles);

	// 验证分裂被阻止
	TestEqual(TEXT("Split should be blocked due to max depth"), Count, 0);
	TestEqual(TEXT("ChildMarbles array should be empty"), ChildMarbles.Num(), 0);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
