// Copyright Echo Alchemist Game. All Rights Reserved.
// 优化版本：使用辅助函数减少重复代码

#include "Combat/CombatSystemInitializer.h"
#include "Combat/CombatManager.h"
#include "Combat/CombatPhysicsIntegrator.h"
#include "Combat/EnemyManager.h"
#include "Combat/CombatSystemHelpers.h"
#include "Physics/MarblePhysicsSystem.h"
#include "Physics/CollisionManager.h"
#include "Physics/PhysicsSceneConfig.h"
#include "Combat/FallingSceneManager.h"
#include "Combat/CircularSceneManager.h"
#include "Combat/CombatBlueprintLibrary.h"

// Initialize static variable
FString UCombatSystemInitializer::LastInitializationError = TEXT("");

UCombatManager* UCombatSystemInitializer::InitializeCombatSystem(
    UObject* WorldContextObject,
    int32 Difficulty,
    ECombatSceneType SceneType
)
{
    // Create config from difficulty
    FCombatConfig Config = UCombatBlueprintLibrary::CreateCombatConfig(Difficulty);
    return InitializeCombatSystemWithConfig(WorldContextObject, Config, SceneType);
}

UCombatManager* UCombatSystemInitializer::InitializeCombatSystemWithConfig(
    UObject* WorldContextObject,
    const FCombatConfig& Config,
    ECombatSceneType SceneType
)
{
    using namespace CombatSystemHelpers;
    
    LastInitializationError.Empty();

    if (!WorldContextObject)
    {
        LogInitializationError(TEXT("WorldContextObject is null."));
        return nullptr;
    }

    // 1. Create Scene Manager
    TScriptInterface<ISceneManager> SceneManager = CreateSceneManager(WorldContextObject, SceneType);
    if (!SceneManager)
    {
        return nullptr;
    }

    // 2. Create Combat Manager
    FString ErrorMsg;
    UCombatManager* CombatManager = CreateObjectSafe<UCombatManager>(WorldContextObject, ErrorMsg);
    if (!CombatManager)
    {
        LogInitializationError(ErrorMsg);
        return nullptr;
    }
    CombatManager->Initialize(Config, SceneManager);

    // 3. Create Enemy Manager
    UEnemyManager* EnemyManager = CreateObjectSafe<UEnemyManager>(WorldContextObject, ErrorMsg);
    if (!EnemyManager)
    {
        LogInitializationError(ErrorMsg);
        return nullptr;
    }
    EnemyManager->Initialize(SceneManager);

    // 4. Create Physics System
    UMarblePhysicsSystem* PhysicsSystem = CreateObjectSafe<UMarblePhysicsSystem>(WorldContextObject, ErrorMsg);
    if (!PhysicsSystem)
    {
        LogInitializationError(ErrorMsg);
        return nullptr;
    }
    // Initialize with combat scene config
    FPhysicsSceneConfig PhysicsConfig;
    PhysicsConfig.SceneType = EPhysicsSceneType::Default;
    PhysicsConfig.BoundsMin = FVector(-1000.0f, -1000.0f, -1000.0f);
    PhysicsConfig.BoundsMax = FVector(1000.0f, 1000.0f, 1000.0f);
    PhysicsConfig.GravityDirection = FVector(0, 0, -1);
    PhysicsConfig.GravityStrength = 980.0f;
    PhysicsSystem->InitializeScene(PhysicsConfig);

    // 5. Create Collision Manager
    UCollisionManager* CollisionManager = CreateObjectSafe<UCollisionManager>(WorldContextObject, ErrorMsg);
    if (!CollisionManager)
    {
        LogInitializationError(ErrorMsg);
        return nullptr;
    }
    CollisionManager->Initialize(FVector(-1000.0f, -1000.0f, -1000.0f), FVector(1000.0f, 1000.0f, 1000.0f), 100.0f);

    // 6. Create Physics Integrator
    UCombatPhysicsIntegrator* Integrator = CreateObjectSafe<UCombatPhysicsIntegrator>(WorldContextObject, ErrorMsg);
    if (!Integrator)
    {
        LogInitializationError(ErrorMsg);
        return nullptr;
    }
    Integrator->Initialize(CombatManager, EnemyManager, PhysicsSystem, CollisionManager);

    // 7. Set Integrator in Combat Manager
    CombatManager->SetPhysicsIntegrator(Integrator);

    UE_LOG(LogTemp, Log, TEXT("Combat System Initialized Successfully."));

    return CombatManager;
}

bool UCombatSystemInitializer::IsCombatSystemValid(UCombatManager* CombatManager)
{
    if (!CombatManager)
    {
        return false;
    }

    if (!CombatManager->GetSceneManager().GetInterface())
    {
        return false;
    }

    if (!CombatManager->GetPhysicsIntegrator())
    {
        return false;
    }

    // Enhanced validation: check integrator sub-systems
    UCombatPhysicsIntegrator* Integrator = CombatManager->GetPhysicsIntegrator();
    
    // Note: These getters would need to be added to CombatPhysicsIntegrator
    // For now, we assume if the integrator exists, it's valid
    
    return true;
}

FString UCombatSystemInitializer::GetLastInitializationError()
{
    return LastInitializationError;
}

TScriptInterface<ISceneManager> UCombatSystemInitializer::CreateSceneManager(UObject* WorldContextObject, ECombatSceneType SceneType)
{
    UClass* SceneManagerClass = nullptr;
    if (SceneType == ECombatSceneType::Falling)
    {
        SceneManagerClass = UFallingSceneManager::StaticClass();
    }
    else if (SceneType == ECombatSceneType::Circular)
    {
        SceneManagerClass = UCircularSceneManager::StaticClass();
    }
    else
    {
        LogInitializationError(FString::Printf(TEXT("Invalid SceneType: %d"), SceneType));
        return nullptr;
    }

    UObject* SceneManagerObject = NewObject<UObject>(WorldContextObject, SceneManagerClass);
    if (!SceneManagerObject)
    {
        LogInitializationError(FString::Printf(TEXT("Failed to create SceneManager of type %s."), *SceneManagerClass->GetName()));
        return nullptr;
    }

    TScriptInterface<ISceneManager> SceneManager;
    SceneManager.SetObject(SceneManagerObject);
    SceneManager.SetInterface(Cast<ISceneManager>(SceneManagerObject));

    if (!SceneManager)
    {
        LogInitializationError(FString::Printf(TEXT("Failed to cast SceneManager object to ISceneManager interface.")));
        return nullptr;
    }

    return SceneManager;
}

void UCombatSystemInitializer::LogInitializationError(const FString& ErrorMessage)
{
    LastInitializationError = ErrorMessage;
    UE_LOG(LogTemp, Error, TEXT("CombatSystemInitializer: %s"), *ErrorMessage);
}
