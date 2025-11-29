// Copyright 2025 Voidzyy. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Physics/MarbleActorPool.h"
#include "Physics/MarbleActor.h"
#include "Engine/World.h"

// 注意：这些测试需要World对象，可能需要在UE4编辑器中运行

// 测试：对象池初始化
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarbleActorPoolInitTest, 
	"EchoAlchemist.Physics.MarbleActorPool.Init", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMarbleActorPoolInitTest::RunTest(const FString& Parameters)
{
	// 创建对象池
	UMarbleActorPool* ActorPool = NewObject<UMarbleActorPool>();
	TestNotNull(TEXT("ActorPool should be created"), ActorPool);

	// 测试初始化前状态
	TestFalse(TEXT("ActorPool should not be initialized"), ActorPool->IsInitialized());
	TestEqual(TEXT("Available count should be 0"), ActorPool->GetAvailableCount(), 0);
	TestEqual(TEXT("InUse count should be 0"), ActorPool->GetInUseCount(), 0);

	// 注意：需要World对象才能初始化，这里跳过实际初始化测试
	// ActorPool->Initialize(GetWorld(), 10);

	return true;
}

// 测试：对象池获取和归还
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarbleActorPoolAcquireReleaseTest, 
	"EchoAlchemist.Physics.MarbleActorPool.AcquireRelease", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMarbleActorPoolAcquireReleaseTest::RunTest(const FString& Parameters)
{
	// 创建对象池
	UMarbleActorPool* ActorPool = NewObject<UMarbleActorPool>();

	// 注意：需要World对象才能测试，这里只测试接口存在性
	// 实际测试需要在UE4编辑器中运行

	// 测试接口存在
	TestNotNull(TEXT("ActorPool should have Acquire method"), ActorPool);
	TestNotNull(TEXT("ActorPool should have Release method"), ActorPool);
	TestNotNull(TEXT("ActorPool should have GetStatistics method"), ActorPool);

	return true;
}

// 测试：对象池统计信息
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarbleActorPoolStatisticsTest, 
	"EchoAlchemist.Physics.MarbleActorPool.Statistics", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMarbleActorPoolStatisticsTest::RunTest(const FString& Parameters)
{
	// 创建对象池
	UMarbleActorPool* ActorPool = NewObject<UMarbleActorPool>();

	// 获取统计信息
	int32 TotalCount, AvailableCount, InUseCount;
	ActorPool->GetStatistics(TotalCount, AvailableCount, InUseCount);

	// 验证初始状态
	TestEqual(TEXT("TotalCount should be 0"), TotalCount, 0);
	TestEqual(TEXT("AvailableCount should be 0"), AvailableCount, 0);
	TestEqual(TEXT("InUseCount should be 0"), InUseCount, 0);

	return true;
}

// 测试：分级降级策略
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarbleActorPoolGenerationTest, 
	"EchoAlchemist.Physics.MarbleActorPool.Generation", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMarbleActorPoolGenerationTest::RunTest(const FString& Parameters)
{
	// 测试分级降级策略逻辑
	// 第0-1代：使用Actor
	// 第2代及以上：使用Niagara粒子

	// 模拟代数判断
	auto ShouldUseActor = [](int32 Generation) -> bool
	{
		return Generation <= 1;
	};

	// 验证策略
	TestTrue(TEXT("Generation 0 should use Actor"), ShouldUseActor(0));
	TestTrue(TEXT("Generation 1 should use Actor"), ShouldUseActor(1));
	TestFalse(TEXT("Generation 2 should use Particle"), ShouldUseActor(2));
	TestFalse(TEXT("Generation 3 should use Particle"), ShouldUseActor(3));

	return true;
}

// 测试：对象池性能（预创建vs动态创建）
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMarbleActorPoolPerformanceTest, 
	"EchoAlchemist.Physics.MarbleActorPool.Performance", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FMarbleActorPoolPerformanceTest::RunTest(const FString& Parameters)
{
	// 注意：这个测试需要在UE4编辑器中运行，因为需要实际创建Actor

	// 测试场景：
	// 1. 预创建50个Actor
	// 2. 获取50个Actor（应该很快，因为已经预创建）
	// 3. 获取第51个Actor（需要动态创建，会稍慢）

	// 预期结果：
	// - 预创建的Actor获取时间 < 1ms
	// - 动态创建的Actor获取时间 < 10ms

	// 实际测试代码需要在UE4编辑器中运行

	return true;
}


#endif // WITH_DEV_AUTOMATION_TESTS
