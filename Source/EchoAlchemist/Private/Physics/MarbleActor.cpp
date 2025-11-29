// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Physics/MarbleActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AMarbleActor::AMarbleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// 创建球体碰撞组件
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(10.0f);
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetEnableGravity(true);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);

	// 创建静态网格组件
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 生成唯一ID
	MarbleID = FGuid::NewGuid();
}

void AMarbleActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMarbleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 更新缓存状态
	if (!bIsInPool)
	{
		CachedState.Position = GetActorLocation();
		CachedState.Velocity = GetVelocity();
		CachedState.LastUpdateTime += DeltaTime;
	}
}

void AMarbleActor::InitializeFromState(const FMarbleState& State)
{
	// 保存状态
	CachedState = State;
	MarbleID = State.ID;

	// 设置位置
	SetActorLocation(State.Position);

	// 设置半径
	if (SphereComponent)
	{
		SphereComponent->SetSphereRadius(State.Radius);
		
		// 设置质量
		SphereComponent->SetMassOverrideInKg(NAME_None, State.Mass, true);
	}

	// 设置网格缩放
	if (MeshComponent)
	{
		float Scale = State.Radius / 10.0f;  // 假设默认网格半径为10cm
		MeshComponent->SetWorldScale3D(FVector(Scale));
	}

	// 设置初始速度
	if (SphereComponent && State.Velocity.SizeSquared() > 0.0f)
	{
		SphereComponent->SetPhysicsLinearVelocity(State.Velocity);
	}

	// 标记为激活状态
	bIsInPool = false;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	UE_LOG(LogTemp, Verbose, TEXT("[MarbleActor] Initialized: ID=%s, Radius=%.2f, Mass=%.2f"),
		*MarbleID.ToString(), State.Radius, State.Mass);
}

void AMarbleActor::Launch(FVector Direction, float Speed)
{
	if (!SphereComponent)
	{
		return;
	}

	// 归一化方向
	FVector NormalizedDirection = Direction.GetSafeNormal();

	// 计算目标速度
	FVector TargetVelocity = NormalizedDirection * Speed;

	// 应用速度
	SphereComponent->SetPhysicsLinearVelocity(TargetVelocity);

	UE_LOG(LogTemp, Verbose, TEXT("[MarbleActor] Launched: ID=%s, Speed=%.2f"),
		*MarbleID.ToString(), Speed);
}

FMarbleState AMarbleActor::GetCurrentState() const
{
	FMarbleState State = CachedState;
	State.Position = GetActorLocation();
	State.Velocity = GetVelocity();
	return State;
}

FVector AMarbleActor::GetVelocity() const
{
	if (SphereComponent)
	{
		return SphereComponent->GetPhysicsLinearVelocity();
	}
	return FVector::ZeroVector;
}

float AMarbleActor::GetSpeed() const
{
	return GetVelocity().Size();
}

void AMarbleActor::SetGravityEnabled(bool bEnable)
{
	if (SphereComponent)
	{
		SphereComponent->SetEnableGravity(bEnable);
	}
}

void AMarbleActor::ApplyImpulse(FVector Impulse)
{
	if (SphereComponent)
	{
		SphereComponent->AddImpulse(Impulse);
	}
}

void AMarbleActor::ResetToPool()
{
	// 停止物理模拟
	if (SphereComponent)
	{
		SphereComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
		SphereComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}

	// 隐藏Actor
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// 标记为在对象池中
	bIsInPool = true;

	// 重置状态
	CachedState = FMarbleState();
	MarbleID = FGuid::NewGuid();

	UE_LOG(LogTemp, Verbose, TEXT("[MarbleActor] Reset to pool"));
}
