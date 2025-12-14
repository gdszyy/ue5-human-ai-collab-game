// Copyright 2025 Voidzyy. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Physics/MarblePhysicsSystem.h"
#include "Physics/PhysicsSceneConfig.h"

// 测试：场景初始化
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarblePhysicsSystemInitTest, 
	"EchoAlchemist.Physics.MarblePhysicsSystem.Init", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMarblePhysicsSystemInitTest::RunTest(const FString& Parameters)
{
	// 创建物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	TestNotNull(TEXT("PhysicsSystem should be created"), PhysicsSystem);

	// 测试初始化前状态
	TestFalse(TEXT("PhysicsSystem should not be initialized"), PhysicsSystem->IsInitialized());

	// 创建战斗场景配置
	FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000)
	);

	// 初始化场景
	PhysicsSystem->InitializeScene(Config);

	// 测试初始化后状态
	TestTrue(TEXT("PhysicsSystem should be initialized"), PhysicsSystem->IsInitialized());
	TestEqual(TEXT("Scene type should be Combat"), PhysicsSystem->GetSceneType(), EPhysicsSceneType::PhysicsScene_Combat);
	TestEqual(TEXT("Marble count should be 0"), PhysicsSystem->GetMarbleCount(), 0);

	return true;
}

// 测试：魔力露珠发射
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarblePhysicsSystemLaunchTest, 
	"EchoAlchemist.Physics.MarblePhysicsSystem.Launch", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMarblePhysicsSystemLaunchTest::RunTest(const FString& Parameters)
{
	// 创建并初始化物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000)
	);
	PhysicsSystem->InitializeScene(Config);

	// 创建发射参数
	FMarbleLaunchParams Params;
	Params.LaunchPosition = FVector(0, 0, 100);
	Params.LaunchDirection = FVector(1, 0, 0);
	Params.LaunchSpeed = 1000.0f;
	Params.Radius = 10.0f;
	Params.Mass = 1.0f;
	Params.Potency = 5.0f;

	// 发射魔力露珠
	FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);

	// 验证发射结果
	TestTrue(TEXT("MarbleID should be valid"), MarbleID.IsValid());
	TestEqual(TEXT("Marble count should be 1"), PhysicsSystem->GetMarbleCount(), 1);

	// 获取魔力露珠状态
	FMarbleState State;
	bool bFound = PhysicsSystem->GetMarbleState(MarbleID, State);
	TestTrue(TEXT("Marble state should be found"), bFound);
	TestEqual(TEXT("Position should match"), State.Position, Params.LaunchPosition);
	TestEqual(TEXT("Velocity should match"), State.Velocity, Params.LaunchDirection * Params.LaunchSpeed);
	TestEqual(TEXT("Radius should match"), State.Radius, Params.Radius);
	TestEqual(TEXT("Mass should match"), State.Mass, Params.Mass);
	TestEqual(TEXT("Potency should match"), State.Potency, Params.Potency);

	return true;
}

// 测试：物理更新循环
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarblePhysicsSystemTickTest, 
	"EchoAlchemist.Physics.MarblePhysicsSystem.Tick", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMarblePhysicsSystemTickTest::RunTest(const FString& Parameters)
{
	// 创建并初始化物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000)
	);
	PhysicsSystem->InitializeScene(Config);

	// 发射魔力露珠
	FMarbleLaunchParams Params;
	Params.LaunchPosition = FVector(0, 0, 100);
	Params.LaunchDirection = FVector(1, 0, 0);
	Params.LaunchSpeed = 1000.0f;
	Params.Radius = 10.0f;
	Params.Mass = 1.0f;
	Params.Potency = 5.0f;
	FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);

	// 记录初始位置
	FMarbleState InitialState;
	PhysicsSystem->GetMarbleState(MarbleID, InitialState);

	// 更新物理（1秒）
	float DeltaTime = 1.0f;
	PhysicsSystem->Tick(DeltaTime);

	// 获取更新后状态
	FMarbleState UpdatedState;
	PhysicsSystem->GetMarbleState(MarbleID, UpdatedState);

	// 验证位置已更新（战斗场景无重力，应该沿初始方向移动）
	FVector ExpectedPosition = InitialState.Position + InitialState.Velocity * DeltaTime;
	TestEqual(TEXT("Position should be updated"), UpdatedState.Position, ExpectedPosition);

	return true;
}

// 测试：边界处理（战斗场景反弹）
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarblePhysicsSystemBoundaryTest, 
	"EchoAlchemist.Physics.MarblePhysicsSystem.Boundary", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMarblePhysicsSystemBoundaryTest::RunTest(const FString& Parameters)
{
	// 创建并初始化物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(
		FVector(-100, -100, 0),
		FVector(100, 100, 100)
	);
	PhysicsSystem->InitializeScene(Config);

	// 发射魔力露珠（向右边界移动）
	FMarbleLaunchParams Params;
	Params.LaunchPosition = FVector(90, 0, 50);
	Params.LaunchDirection = FVector(1, 0, 0);
	Params.LaunchSpeed = 100.0f;
	Params.Radius = 10.0f;
	Params.Mass = 1.0f;
	Params.Potency = 5.0f;
	FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);

	// 记录初始速度
	FMarbleState InitialState;
	PhysicsSystem->GetMarbleState(MarbleID, InitialState);
	FVector InitialVelocity = InitialState.Velocity;

	// 更新物理（足够长的时间让弹珠碰到边界）
	for (int32 i = 0; i < 10; ++i)
	{
		PhysicsSystem->Tick(0.1f);
	}

	// 获取更新后状态
	FMarbleState UpdatedState;
	PhysicsSystem->GetMarbleState(MarbleID, UpdatedState);

	// 验证速度已反弹（X方向应该反向）
	TestTrue(TEXT("Velocity X should be reversed"), UpdatedState.Velocity.X < 0);

	return true;
}

// 测试：药效强度消耗
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarblePhysicsSystemPotencyTest, 
	"EchoAlchemist.Physics.MarblePhysicsSystem.Potency", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMarblePhysicsSystemPotencyTest::RunTest(const FString& Parameters)
{
	// 创建并初始化物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000)
	);
	PhysicsSystem->InitializeScene(Config);

	// 发射魔力露珠
	FMarbleLaunchParams Params;
	Params.LaunchPosition = FVector(0, 0, 100);
	Params.LaunchDirection = FVector(1, 0, 0);
	Params.LaunchSpeed = 1000.0f;
	Params.Radius = 10.0f;
	Params.Mass = 1.0f;
	Params.Potency = 3.0f;  // 初始药效强度为3
	FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);

	// 模拟碰撞消耗药效（手动调用，实际应该由碰撞检测系统触发）
	// 注意：这里需要物理系统提供ConsumePotency接口，暂时跳过
	// PhysicsSystem->ConsumePotency(MarbleID, 1.0f);

	// TODO: 等待物理系统提供药效消耗接口后补充测试

	return true;
}

// 测试：多个魔力露珠
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarblePhysicsSystemMultipleTest, 
	"EchoAlchemist.Physics.MarblePhysicsSystem.Multiple", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMarblePhysicsSystemMultipleTest::RunTest(const FString& Parameters)
{
	// 创建并初始化物理系统
	UMarblePhysicsSystem* PhysicsSystem = NewObject<UMarblePhysicsSystem>();
	FPhysicsSceneConfig Config = USceneConfigFactory::CreateCombatConfig(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000)
	);
	PhysicsSystem->InitializeScene(Config);

	// 发射多个魔力露珠
	TArray<FGuid> MarbleIDs;
	for (int32 i = 0; i < 10; ++i)
	{
		FMarbleLaunchParams Params;
		Params.LaunchPosition = FVector(i * 100.0f, 0, 100);
		Params.LaunchDirection = FVector(1, 0, 0);
		Params.LaunchSpeed = 1000.0f;
		Params.Radius = 10.0f;
		Params.Mass = 1.0f;
		Params.Potency = 5.0f;
		
		FGuid MarbleID = PhysicsSystem->LaunchMarble(Params);
		MarbleIDs.Add(MarbleID);
	}

	// 验证数量
	TestEqual(TEXT("Marble count should be 10"), PhysicsSystem->GetMarbleCount(), 10);

	// 获取所有魔力露珠
	TArray<FMarbleState> AllMarbles = PhysicsSystem->GetAllMarbles();
	TestEqual(TEXT("GetAllMarbles should return 10 marbles"), AllMarbles.Num(), 10);

	// 更新物理
	PhysicsSystem->Tick(1.0f);

	// 验证所有魔力露珠都已更新
	for (const FGuid& MarbleID : MarbleIDs)
	{
		FMarbleState State;
		bool bFound = PhysicsSystem->GetMarbleState(MarbleID, State);
		TestTrue(TEXT("Marble should exist"), bFound);
	}

	return true;
}


#endif // WITH_DEV_AUTOMATION_TESTS
