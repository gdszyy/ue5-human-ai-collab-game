// Copyright 2025 Voidzyy. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Physics/SpecialEffectSystem.h"
#include "Physics/SpecialEffectBlueprintLibrary.h"

// 测试：特殊效果系统初始化
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectSystemInitTest,
	"EchoAlchemist.Physics.SpecialEffectSystem.Init",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectSystemInitTest::RunTest(const FString& Parameters)
{
	// 创建特殊效果系统
	USpecialEffectSystem* EffectSystem = NewObject<USpecialEffectSystem>();
	TestNotNull(TEXT("EffectSystem should be created"), EffectSystem);

	// 测试初始化前状态
	TestFalse(TEXT("EffectSystem should not be initialized"), EffectSystem->IsInitialized());

	// 初始化
	EffectSystem->Initialize();

	// 测试初始化后状态
	TestTrue(TEXT("EffectSystem should be initialized"), EffectSystem->IsInitialized());
	TestEqual(TEXT("Effect count should be 0"), EffectSystem->GetEffectCountByType(EEchoSpecialEffectType::GravityWell), 0);

	return true;
}

// 测试：创建引力奇点
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectGravityWellTest,
	"EchoAlchemist.Physics.SpecialEffectSystem.GravityWell",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectGravityWellTest::RunTest(const FString& Parameters)
{
	// 创建并初始化系统
	USpecialEffectSystem* EffectSystem = NewObject<USpecialEffectSystem>();
	EffectSystem->Initialize();

	// 创建引力奇点参数
	FGravityWellParams Params = USpecialEffectBlueprintLibrary::MakeGravityWellParams(
		FVector(0, 0, 100),
		1000.0f,
		500.0f,
		5.0f,
		false
	);

	// 创建引力奇点
	FGuid EffectID = EffectSystem->CreateGravityWell(Params);

	// 验证
	TestTrue(TEXT("EffectID should be valid"), EffectID.IsValid());
	TestEqual(TEXT("GravityWell count should be 1"), EffectSystem->GetEffectCountByType(EEchoSpecialEffectType::GravityWell), 1);

	// 获取所有效果
	TArray<FSpecialEffectData> Effects = EffectSystem->GetAllActiveEffects();
	TestEqual(TEXT("Active effects count should be 1"), Effects.Num(), 1);
	TestTrue(TEXT("Effect type should be GravityWell"), Effects[0].EffectType == EEchoSpecialEffectType::GravityWell);

	return true;
}

// 测试：创建虫洞
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectWormholeTest,
	"EchoAlchemist.Physics.SpecialEffectSystem.Wormhole",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectWormholeTest::RunTest(const FString& Parameters)
{
	// 创建并初始化系统
	USpecialEffectSystem* EffectSystem = NewObject<USpecialEffectSystem>();
	EffectSystem->Initialize();

	// 创建虫洞参数
	FWormholeParams Params = USpecialEffectBlueprintLibrary::MakeWormholeParams(
		FVector(0, 0, 100),
		FVector(500, 0, 100),
		50.0f,
		1.0f,
		10.0f
	);

	// 创建虫洞
	FGuid EffectID = EffectSystem->CreateWormhole(Params);

	// 验证
	TestTrue(TEXT("EffectID should be valid"), EffectID.IsValid());
	TestEqual(TEXT("Wormhole count should be 1"), EffectSystem->GetEffectCountByType(EEchoSpecialEffectType::Wormhole), 1);

	return true;
}

// 测试：弹珠分裂效果
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectSplitTest,
	"EchoAlchemist.Physics.SpecialEffectSystem.Split",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectSplitTest::RunTest(const FString& Parameters)
{
	// 创建并初始化系统
	USpecialEffectSystem* EffectSystem = NewObject<USpecialEffectSystem>();
	EffectSystem->Initialize();

	// 创建测试弹珠
	FMarbleState Marble;
	Marble.ID = FGuid::NewGuid();
	Marble.Position = FVector(0, 0, 100);
	Marble.Velocity = FVector(1000, 0, 0);
	Marble.Radius = 10.0f;
	Marble.Mass = 1.0f;
	Marble.Potency = 5.0f;
	Marble.Generation = 0;

	// 创建分裂参数
	FSplitParams Params = USpecialEffectBlueprintLibrary::MakeBinarySplit();

	// 应用分裂效果
	TArray<FMarbleState> NewMarbles;
	int32 SplitCount = EffectSystem->ApplySplitEffect(Marble, Params, NewMarbles);

	// 验证
	TestEqual(TEXT("Split count should be 2"), SplitCount, 2);
	TestEqual(TEXT("NewMarbles count should be 2"), NewMarbles.Num(), 2);

	// 验证子弹珠属性
	for (const FMarbleState& NewMarble : NewMarbles)
	{
		TestTrue(TEXT("NewMarble ID should be valid"), NewMarble.ID.IsValid());
		TestTrue(TEXT("NewMarble ID should be different from parent"), NewMarble.ID != Marble.ID);
		TestEqual(TEXT("NewMarble generation should be parent+1"), NewMarble.Generation, Marble.Generation + 1);
		TestTrue(TEXT("NewMarble radius should be smaller"), NewMarble.Radius < Marble.Radius);
		TestTrue(TEXT("NewMarble potency should be smaller"), NewMarble.Potency < Marble.Potency);
	}

	return true;
}

// 测试：速度修改效果
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectSpeedModifierTest,
	"EchoAlchemist.Physics.SpecialEffectSystem.SpeedModifier",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectSpeedModifierTest::RunTest(const FString& Parameters)
{
	// 创建并初始化系统
	USpecialEffectSystem* EffectSystem = NewObject<USpecialEffectSystem>();
	EffectSystem->Initialize();

	// 创建测试弹珠
	FMarbleState Marble;
	Marble.ID = FGuid::NewGuid();
	Marble.Position = FVector(0, 0, 100);
	Marble.Velocity = FVector(1000, 0, 0);
	Marble.Radius = 10.0f;

	float OriginalSpeed = Marble.Velocity.Size();

	// 创建加速参数
	FEchoSpeedModifierParams Params = USpecialEffectBlueprintLibrary::MakeSpeedBoost();

	// 应用速度修改
	FMarbleState ModifiedMarble;
	bool bSuccess = EffectSystem->ApplySpeedModifier(Marble, Params, ModifiedMarble);

	// 验证
	TestTrue(TEXT("Speed modifier should succeed"), bSuccess);
	
	float NewSpeed = ModifiedMarble.Velocity.Size();
	float ExpectedSpeed = OriginalSpeed * Params.SpeedMultiplier;
	
	TestTrue(TEXT("Speed should be increased"), NewSpeed > OriginalSpeed);
	TestEqual(TEXT("Speed should match expected value"), NewSpeed, ExpectedSpeed, 0.1f);

	return true;
}

// 测试：连锁触发效果
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectChainTriggerTest,
	"EchoAlchemist.Physics.SpecialEffectSystem.ChainTrigger",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectChainTriggerTest::RunTest(const FString& Parameters)
{
	// 创建并初始化系统
	USpecialEffectSystem* EffectSystem = NewObject<USpecialEffectSystem>();
	EffectSystem->Initialize();

	// 创建连锁触发参数
	FChainTriggerParams Params = USpecialEffectBlueprintLibrary::MakeChainTriggerParams(
		3,        // SecondaryCount
		60.0f,    // AngleRange
		800.0f,   // SecondarySpeed
		0.3f,     // PotencyMultiplier
		5.0f      // SecondaryRadius
	);

	// 应用连锁触发
	TArray<FMarbleState> SecondaryMarbles;
	int32 SecondaryCount = EffectSystem->ApplyChainTrigger(
		FVector(0, 0, 100),
		FVector(1, 0, 0),
		Params,
		SecondaryMarbles
	);

	// 验证
	TestEqual(TEXT("Secondary count should be 3"), SecondaryCount, 3);
	TestEqual(TEXT("SecondaryMarbles count should be 3"), SecondaryMarbles.Num(), 3);

	// 验证次级弹珠属性
	for (const FMarbleState& Secondary : SecondaryMarbles)
	{
		TestTrue(TEXT("Secondary ID should be valid"), Secondary.ID.IsValid());
		TestEqual(TEXT("Secondary radius should match params"), Secondary.Radius, Params.SecondaryRadius);
		
		float Speed = Secondary.Velocity.Size();
		TestEqual(TEXT("Secondary speed should match params"), Speed, Params.SecondarySpeed, 1.0f);
	}

	return true;
}

// 测试：效果过期移除
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectExpirationTest,
	"EchoAlchemist.Physics.SpecialEffectSystem.Expiration",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectExpirationTest::RunTest(const FString& Parameters)
{
	// 创建并初始化系统
	USpecialEffectSystem* EffectSystem = NewObject<USpecialEffectSystem>();
	EffectSystem->Initialize();

	// 创建短时效引力奇点
	FGravityWellParams Params = USpecialEffectBlueprintLibrary::MakeGravityWellParams(
		FVector(0, 0, 100),
		1000.0f,
		500.0f,
		1.0f,  // 1秒持续时间
		false
	);

	FGuid EffectID = EffectSystem->CreateGravityWell(Params);
	TestEqual(TEXT("GravityWell count should be 1"), EffectSystem->GetEffectCountByType(EEchoSpecialEffectType::GravityWell), 1);

	// 模拟时间流逝（2秒）
	TArray<FMarbleState> EmptyMarbles;
	TArray<FMarbleState> ModifiedMarbles;
	
	EffectSystem->Tick(2.0f, EmptyMarbles, ModifiedMarbles);

	// 验证效果已过期
	TestEqual(TEXT("GravityWell should be removed after expiration"), 
		EffectSystem->GetEffectCountByType(EEchoSpecialEffectType::GravityWell), 0);

	return true;
}

// 测试：蓝图函数库预设参数
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpecialEffectBlueprintLibraryTest,
	"EchoAlchemist.Physics.SpecialEffectSystem.BlueprintLibrary",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpecialEffectBlueprintLibraryTest::RunTest(const FString& Parameters)
{
	// 测试预设参数
	FGravityWellParams WeakGravity = USpecialEffectBlueprintLibrary::MakeWeakGravityWell(FVector::ZeroVector);
	TestTrue(TEXT("WeakGravity strength should be less than 1000"), WeakGravity.GravityStrength < 1000.0f);

	FGravityWellParams StrongGravity = USpecialEffectBlueprintLibrary::MakeStrongGravityWell(FVector::ZeroVector);
	TestTrue(TEXT("StrongGravity strength should be greater than 1000"), StrongGravity.GravityStrength > 1000.0f);

	FSplitParams BinarySplit = USpecialEffectBlueprintLibrary::MakeBinarySplit();
	TestEqual(TEXT("BinarySplit count should be 2"), BinarySplit.SplitCount, 2);

	FSplitParams TripleSplit = USpecialEffectBlueprintLibrary::MakeTripleSplit();
	TestEqual(TEXT("TripleSplit count should be 3"), TripleSplit.SplitCount, 3);

	FEchoSpeedModifierParams SpeedBoost = USpecialEffectBlueprintLibrary::MakeSpeedBoost();
	TestTrue(TEXT("SpeedBoost multiplier should be greater than 1"), SpeedBoost.SpeedMultiplier > 1.0f);

	FEchoSpeedModifierParams SpeedSlow = USpecialEffectBlueprintLibrary::MakeSpeedSlow();
	TestTrue(TEXT("SpeedSlow multiplier should be less than 1"), SpeedSlow.SpeedMultiplier < 1.0f);

	// 测试辅助函数
	FString GravityWellName = USpecialEffectBlueprintLibrary::GetEffectTypeName(EEchoSpecialEffectType::GravityWell);
	TestEqual(TEXT("GravityWell name should be correct"), GravityWellName, FString(TEXT("Gravity Well")));

	bool bIsContinuous = USpecialEffectBlueprintLibrary::IsContinuousEffect(EEchoSpecialEffectType::GravityWell);
	TestTrue(TEXT("GravityWell should be continuous effect"), bIsContinuous);

	bool bIsInstant = USpecialEffectBlueprintLibrary::IsContinuousEffect(EEchoSpecialEffectType::Split);
	TestFalse(TEXT("Split should not be continuous effect"), bIsInstant);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
