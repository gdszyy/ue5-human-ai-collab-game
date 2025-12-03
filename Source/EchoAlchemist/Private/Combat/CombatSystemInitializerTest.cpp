// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/CombatSystemInitializer.h"
#include "Combat/CombatManager.h"
#include "Misc/AutomationTest.h"

#if WITH_AUTOMATION_TESTS

/**
 * 战斗系统初始化器单元测试
 */
class FCombatSystemInitializerTest : public FAutomationTestBase
{
public:
    FCombatSystemInitializerTest(const FString& InName, const bool bInComplexTask)
        : FAutomationTestBase(InName, bInComplexTask)
    {
    }

    virtual uint32 GetTestFlags() const override
    {
        return EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter;
    }

    virtual bool RunTest(const FString& Parameters) override;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCombatSystemInitializerSimpleTest,
    "EchoAlchemist.Combat.CombatSystemInitializer.Simple",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter
)

bool FCombatSystemInitializerSimpleTest::RunTest(const FString& Parameters)
{
    // 测试1：简单初始化
    {
        UCombatManager* CombatManager = UCombatSystemInitializer::InitializeCombatSystem(
            GetWorld(),
            5,
            ECombatSceneType::Falling
        );

        TestTrue("Combat Manager created", CombatManager != nullptr);
        TestTrue("Combat System is valid", UCombatSystemInitializer::IsCombatSystemValid(CombatManager));
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCombatSystemInitializerDifficultyTest,
    "EchoAlchemist.Combat.CombatSystemInitializer.Difficulty",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter
)

bool FCombatSystemInitializerDifficultyTest::RunTest(const FString& Parameters)
{
    // 测试2：不同难度等级的初始化
    {
        // 简单难度
        UCombatManager* EasyCombatManager = UCombatSystemInitializer::InitializeCombatSystem(
            GetWorld(),
            1,
            ECombatSceneType::Falling
        );
        TestTrue("Easy Combat System is valid", UCombatSystemInitializer::IsCombatSystemValid(EasyCombatManager));

        // 普通难度
        UCombatManager* NormalCombatManager = UCombatSystemInitializer::InitializeCombatSystem(
            GetWorld(),
            5,
            ECombatSceneType::Falling
        );
        TestTrue("Normal Combat System is valid", UCombatSystemInitializer::IsCombatSystemValid(NormalCombatManager));

        // 困难难度
        UCombatManager* HardCombatManager = UCombatSystemInitializer::InitializeCombatSystem(
            GetWorld(),
            10,
            ECombatSceneType::Falling
        );
        TestTrue("Hard Combat System is valid", UCombatSystemInitializer::IsCombatSystemValid(HardCombatManager));
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCombatSystemInitializerSceneTypeTest,
    "EchoAlchemist.Combat.CombatSystemInitializer.SceneType",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter
)

bool FCombatSystemInitializerSceneTypeTest::RunTest(const FString& Parameters)
{
    // 测试3：不同场景类型的初始化
    {
        // 下落式场景
        UCombatManager* FallingCombatManager = UCombatSystemInitializer::InitializeCombatSystem(
            GetWorld(),
            5,
            ECombatSceneType::Falling
        );
        TestTrue("Falling Combat System is valid", UCombatSystemInitializer::IsCombatSystemValid(FallingCombatManager));

        // 环形场景
        UCombatManager* CircularCombatManager = UCombatSystemInitializer::InitializeCombatSystem(
            GetWorld(),
            5,
            ECombatSceneType::Circular
        );
        TestTrue("Circular Combat System is valid", UCombatSystemInitializer::IsCombatSystemValid(CircularCombatManager));
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCombatSystemInitializerErrorHandlingTest,
    "EchoAlchemist.Combat.CombatSystemInitializer.ErrorHandling",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter
)

bool FCombatSystemInitializerErrorHandlingTest::RunTest(const FString& Parameters)
{
    // 测试4：错误处理
    {
        // 无效的世界上下文
        UCombatManager* InvalidCombatManager = UCombatSystemInitializer::InitializeCombatSystem(
            nullptr,
            5,
            ECombatSceneType::Falling
        );
        TestTrue("Invalid World Context returns nullptr", InvalidCombatManager == nullptr);
        TestTrue("Error message is not empty", !UCombatSystemInitializer::GetLastInitializationError().IsEmpty());
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FCombatSystemInitializerWithConfigTest,
    "EchoAlchemist.Combat.CombatSystemInitializer.WithConfig",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter
)

bool FCombatSystemInitializerWithConfigTest::RunTest(const FString& Parameters)
{
    // 测试5：使用自定义配置的初始化
    {
        FCombatConfig CustomConfig = FCombatConfig::CreateNormalConfig();
        CustomConfig.MaxEnemies = 15;
        CustomConfig.VictoryKillCount = 30;

        UCombatManager* CustomCombatManager = UCombatSystemInitializer::InitializeCombatSystemWithConfig(
            GetWorld(),
            CustomConfig,
            ECombatSceneType::Falling
        );

        TestTrue("Custom Combat System is valid", UCombatSystemInitializer::IsCombatSystemValid(CustomCombatManager));
    }

    return true;
}

#endif // WITH_AUTOMATION_TESTS
