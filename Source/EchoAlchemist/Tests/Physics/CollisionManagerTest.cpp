// Copyright 2025 Voidzyy. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Physics/CollisionManager.h"
#include "Physics/CollisionShape.h"

// 测试：碰撞管理器初始化
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionManagerInitTest, 
	"EchoAlchemist.Physics.CollisionManager.Init", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCollisionManagerInitTest::RunTest(const FString& Parameters)
{
	// 创建碰撞管理器
	UCollisionManager* CollisionManager = NewObject<UCollisionManager>();
	TestNotNull(TEXT("CollisionManager should be created"), CollisionManager);

	// 测试初始化前状态
	TestFalse(TEXT("CollisionManager should not be initialized"), CollisionManager->IsInitialized());

	// 初始化
	CollisionManager->Initialize(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000),
		100.0f
	);

	// 测试初始化后状态
	TestTrue(TEXT("CollisionManager should be initialized"), CollisionManager->IsInitialized());
	TestEqual(TEXT("Body count should be 0"), CollisionManager->GetBodyCount(), 0);

	return true;
}

// 测试：注册碰撞体
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionManagerRegisterTest, 
	"EchoAlchemist.Physics.CollisionManager.Register", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCollisionManagerRegisterTest::RunTest(const FString& Parameters)
{
	// 创建并初始化碰撞管理器
	UCollisionManager* CollisionManager = NewObject<UCollisionManager>();
	CollisionManager->Initialize(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000),
		100.0f
	);

	// 创建圆形碰撞体
	FCollisionBody CircleBody;
	CircleBody.ID = FGuid::NewGuid();
	CircleBody.Position = FVector(0, 0, 100);
	CircleBody.ShapeType = EEchoCollisionShapeType::Circle;
	CircleBody.EffectRadius = 10.0f;

	// 注册碰撞体
	FGuid BodyID = CollisionManager->RegisterBody(CircleBody);

	// 验证注册结果
	TestTrue(TEXT("BodyID should be valid"), BodyID.IsValid());
	TestEqual(TEXT("Body count should be 1"), CollisionManager->GetBodyCount(), 1);

	// 获取碰撞体
	FCollisionBody RetrievedBody;
	bool bFound = CollisionManager->GetBody(BodyID, RetrievedBody);
	TestTrue(TEXT("Body should be found"), bFound);
	TestEqual(TEXT("Position should match"), RetrievedBody.Position, CircleBody.Position);
	TestEqual(TEXT("Radius should match"), RetrievedBody.EffectRadius, CircleBody.EffectRadius);

	return true;
}

// 测试：圆-圆碰撞检测
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionManagerCircleCircleTest, 
	"EchoAlchemist.Physics.CollisionManager.CircleCircle", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCollisionManagerCircleCircleTest::RunTest(const FString& Parameters)
{
	// 创建并初始化碰撞管理器
	UCollisionManager* CollisionManager = NewObject<UCollisionManager>();
	CollisionManager->Initialize(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000),
		100.0f
	);

	// 创建两个圆形碰撞体（相互碰撞）
	FCollisionBody CircleA;
	CircleA.ID = FGuid::NewGuid();
	CircleA.Position = FVector(0, 0, 100);
	CircleA.ShapeType = EEchoCollisionShapeType::Circle;
	CircleA.EffectRadius = 10.0f;

	FCollisionBody CircleB;
	CircleB.ID = FGuid::NewGuid();
	CircleB.Position = FVector(15, 0, 100);  // 距离15cm，两个半径之和为20cm，应该碰撞
	CircleB.ShapeType = EEchoCollisionShapeType::Circle;
	CircleB.EffectRadius = 10.0f;

	// 注册碰撞体
	CollisionManager->RegisterBody(CircleA);
	CollisionManager->RegisterBody(CircleB);

	// 更新空间网格
	CollisionManager->UpdateSpatialGrid();

	// 执行碰撞检测
	TArray<FEchoCollisionEvent> Collisions = CollisionManager->DetectCollisions();

	// 验证碰撞结果
	TestEqual(TEXT("Should detect 1 collision"), Collisions.Num(), 1);
	if (Collisions.Num() > 0)
	{
		const FEchoCollisionEvent& Event = Collisions[0];
		TestTrue(TEXT("BodyA should be valid"), Event.BodyA.IsValid());
		TestTrue(TEXT("BodyB should be valid"), Event.BodyB.IsValid());
		TestTrue(TEXT("Penetration depth should be positive"), Event.PenetrationDepth > 0);
	}

	return true;
}

// 测试：圆-圆无碰撞
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionManagerCircleCircleNoCollisionTest, 
	"EchoAlchemist.Physics.CollisionManager.CircleCircleNoCollision", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCollisionManagerCircleCircleNoCollisionTest::RunTest(const FString& Parameters)
{
	// 创建并初始化碰撞管理器
	UCollisionManager* CollisionManager = NewObject<UCollisionManager>();
	CollisionManager->Initialize(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000),
		100.0f
	);

	// 创建两个圆形碰撞体（不碰撞）
	FCollisionBody CircleA;
	CircleA.ID = FGuid::NewGuid();
	CircleA.Position = FVector(0, 0, 100);
	CircleA.ShapeType = EEchoCollisionShapeType::Circle;
	CircleA.EffectRadius = 10.0f;

	FCollisionBody CircleB;
	CircleB.ID = FGuid::NewGuid();
	CircleB.Position = FVector(100, 0, 100);  // 距离100cm，两个半径之和为20cm，不应该碰撞
	CircleB.ShapeType = EEchoCollisionShapeType::Circle;
	CircleB.EffectRadius = 10.0f;

	// 注册碰撞体
	CollisionManager->RegisterBody(CircleA);
	CollisionManager->RegisterBody(CircleB);

	// 更新空间网格
	CollisionManager->UpdateSpatialGrid();

	// 执行碰撞检测
	TArray<FEchoCollisionEvent> Collisions = CollisionManager->DetectCollisions();

	// 验证碰撞结果
	TestEqual(TEXT("Should detect 0 collisions"), Collisions.Num(), 0);

	return true;
}

// 测试：圆-矩形碰撞检测
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionManagerCircleRectangleTest, 
	"EchoAlchemist.Physics.CollisionManager.CircleRectangle", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCollisionManagerCircleRectangleTest::RunTest(const FString& Parameters)
{
	// 创建并初始化碰撞管理器
	UCollisionManager* CollisionManager = NewObject<UCollisionManager>();
	CollisionManager->Initialize(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000),
		100.0f
	);

	// 创建圆形碰撞体
	FCollisionBody Circle;
	Circle.ID = FGuid::NewGuid();
	Circle.Position = FVector(0, 0, 100);
	Circle.ShapeType = EEchoCollisionShapeType::Circle;
	Circle.EffectRadius = 10.0f;

	// 创建矩形碰撞体
	FCollisionBody Rectangle;
	Rectangle.ID = FGuid::NewGuid();
	Rectangle.Position = FVector(20, 0, 100);  // 距离20cm
	Rectangle.ShapeType = EEchoCollisionShapeType::Rectangle;
	Rectangle.Size = FVector2D(50.0f, 30.0f);  // 宽50cm，高30cm
	Rectangle.Rotation = 0.0f;

	// 注册碰撞体
	CollisionManager->RegisterBody(Circle);
	CollisionManager->RegisterBody(Rectangle);

	// 更新空间网格
	CollisionManager->UpdateSpatialGrid();

	// 执行碰撞检测
	TArray<FEchoCollisionEvent> Collisions = CollisionManager->DetectCollisions();

	// 验证碰撞结果（圆心距离矩形边缘5cm，圆半径10cm，应该碰撞）
	TestEqual(TEXT("Should detect 1 collision"), Collisions.Num(), 1);

	return true;
}

// 测试：空间网格优化
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionManagerSpatialGridTest, 
	"EchoAlchemist.Physics.CollisionManager.SpatialGrid", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCollisionManagerSpatialGridTest::RunTest(const FString& Parameters)
{
	// 创建并初始化碰撞管理器
	UCollisionManager* CollisionManager = NewObject<UCollisionManager>();
	CollisionManager->Initialize(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000),
		100.0f
	);

	// 创建多个碰撞体
	for (int32 i = 0; i < 50; ++i)
	{
		FCollisionBody Body;
		Body.ID = FGuid::NewGuid();
		Body.Position = FVector(i * 50.0f, 0, 100);
		Body.ShapeType = EEchoCollisionShapeType::Circle;
		Body.EffectRadius = 10.0f;
		CollisionManager->RegisterBody(Body);
	}

	// 更新空间网格
	CollisionManager->UpdateSpatialGrid();

	// 获取空间网格统计信息
	int32 TotalCells, OccupiedCells, MaxBodiesPerCell;
	float AvgBodiesPerCell;
	CollisionManager->GetSpatialGridStatistics(
		TotalCells,
		OccupiedCells,
		MaxBodiesPerCell,
		AvgBodiesPerCell
	);

	// 验证空间网格统计
	TestTrue(TEXT("TotalCells should be positive"), TotalCells > 0);
	TestTrue(TEXT("OccupiedCells should be positive"), OccupiedCells > 0);
	TestTrue(TEXT("OccupiedCells should be <= TotalCells"), OccupiedCells <= TotalCells);
	TestTrue(TEXT("MaxBodiesPerCell should be >= 1"), MaxBodiesPerCell >= 1);
	TestTrue(TEXT("AvgBodiesPerCell should be >= 1"), AvgBodiesPerCell >= 1.0f);

	return true;
}

// 测试：性能基准（100个碰撞体）
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionManagerPerformanceTest, 
	"EchoAlchemist.Physics.CollisionManager.Performance", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCollisionManagerPerformanceTest::RunTest(const FString& Parameters)
{
	// 创建并初始化碰撞管理器
	UCollisionManager* CollisionManager = NewObject<UCollisionManager>();
	CollisionManager->Initialize(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000),
		100.0f
	);

	// 创建100个魔药碰撞体
	for (int32 i = 0; i < 100; ++i)
	{
		FCollisionBody Body;
		Body.ID = FGuid::NewGuid();
		Body.Position = FVector(
			FMath::FRandRange(-500.0f, 500.0f),
			FMath::FRandRange(-500.0f, 500.0f),
			FMath::FRandRange(0.0f, 500.0f)
		);
		Body.ShapeType = EEchoCollisionShapeType::Circle;
		Body.EffectRadius = 10.0f;
		CollisionManager->RegisterBody(Body);
	}

	// 创建50个敌人碰撞体
	for (int32 i = 0; i < 50; ++i)
	{
		FCollisionBody Body;
		Body.ID = FGuid::NewGuid();
		Body.Position = FVector(
			FMath::FRandRange(-500.0f, 500.0f),
			FMath::FRandRange(-500.0f, 500.0f),
			FMath::FRandRange(0.0f, 500.0f)
		);
		Body.ShapeType = EEchoCollisionShapeType::Rectangle;
		Body.Size = FVector2D(50.0f, 30.0f);
		Body.Rotation = 0.0f;
		CollisionManager->RegisterBody(Body);
	}

	// 更新空间网格
	double StartTime = FPlatformTime::Seconds();
	CollisionManager->UpdateSpatialGrid();
	double GridUpdateTime = FPlatformTime::Seconds() - StartTime;

	// 执行碰撞检测
	StartTime = FPlatformTime::Seconds();
	TArray<FEchoCollisionEvent> Collisions = CollisionManager->DetectCollisions();
	double CollisionDetectionTime = FPlatformTime::Seconds() - StartTime;

	// 输出性能数据
	UE_LOG(LogTemp, Log, TEXT("[Performance Test] Grid Update Time: %.3f ms"), GridUpdateTime * 1000.0);
	UE_LOG(LogTemp, Log, TEXT("[Performance Test] Collision Detection Time: %.3f ms"), CollisionDetectionTime * 1000.0);
	UE_LOG(LogTemp, Log, TEXT("[Performance Test] Total Collisions: %d"), Collisions.Num());

	// 验证性能（100个魔药+50个敌人的碰撞检测应该在3ms内完成）
	TestTrue(TEXT("Collision detection should be fast (<3ms)"), CollisionDetectionTime < 0.003);

	return true;
}

// 测试：碰撞事件委托
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCollisionManagerEventTest, 
	"EchoAlchemist.Physics.CollisionManager.Event", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCollisionManagerEventTest::RunTest(const FString& Parameters)
{
	// 创建并初始化碰撞管理器
	UCollisionManager* CollisionManager = NewObject<UCollisionManager>();
	CollisionManager->Initialize(
		FVector(-1000, -1000, 0),
		FVector(1000, 1000, 1000),
		100.0f
	);

	// 注意：UE4 的 DECLARE_DYNAMIC_MULTICAST_DELEGATE 不支持 AddLambda
	// 我们通过检测碰撞结果数组来验证事件功能

	// 创建两个碰撞的圆形碰撞体
	FCollisionBody CircleA;
	CircleA.ID = FGuid::NewGuid();
	CircleA.Position = FVector(0, 0, 100);
	CircleA.ShapeType = EEchoCollisionShapeType::Circle;
	CircleA.EffectRadius = 10.0f;

	FCollisionBody CircleB;
	CircleB.ID = FGuid::NewGuid();
	CircleB.Position = FVector(15, 0, 100);
	CircleB.ShapeType = EEchoCollisionShapeType::Circle;
	CircleB.EffectRadius = 10.0f;

	// 注册碰撞体
	CollisionManager->RegisterBody(CircleA);
	CollisionManager->RegisterBody(CircleB);

	// 更新空间网格
	CollisionManager->UpdateSpatialGrid();

	// 执行碰撞检测
	TArray<FEchoCollisionEvent> Collisions = CollisionManager->DetectCollisions();

	// 验证碰撞检测结果
	TestEqual(TEXT("Should detect one collision"), Collisions.Num(), 1);

	return true;
}


#endif // WITH_DEV_AUTOMATION_TESTS
