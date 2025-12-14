// Copyright 2025 Voidzyy. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Alchemy/RecipeSystem.h"
#include "Alchemy/RecipeBlueprintLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecipeSystemBasicTest, "EchoAlchemist.Alchemy.RecipeSystem.Basic", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecipeSystemBasicTest::RunTest(const FString& Parameters)
{
	// 创建配方系统实例
	URecipeSystem* RecipeSystem = NewObject<URecipeSystem>();
	RecipeSystem->InitializeRecipes();

	// 测试基础配方评估
	FRecipeInput Input;
	Input.BasePotion = EBasePotionType::Ricochet;

	FRecipeOutput Output = RecipeSystem->EvaluateRecipe(Input);

	// 验证配方ID
	TestEqual(TEXT("Recipe ID should be generated"), Output.RecipeID.IsEmpty(), false);

	// 验证基础效果
	TestTrue(TEXT("Should have BaseDamage effect"), Output.Effects.Contains(RecipeEffectKeys::BaseDamage));
	TestTrue(TEXT("Should have Speed effect"), Output.Effects.Contains(RecipeEffectKeys::Speed));

	// 验证配方发现
	TestTrue(TEXT("Should be marked as new recipe"), Output.bIsNewRecipe);
	TestTrue(TEXT("Recipe should be discovered"), RecipeSystem->IsRecipeDiscovered(Output.RecipeID));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecipeSystemAdditiveTest, "EchoAlchemist.Alchemy.RecipeSystem.Additive", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecipeSystemAdditiveTest::RunTest(const FString& Parameters)
{
	URecipeSystem* RecipeSystem = NewObject<URecipeSystem>();
	RecipeSystem->InitializeRecipes();

	// 测试单添加剂配方
	FRecipeInput Input;
	Input.BasePotion = EBasePotionType::Ricochet;
	Input.Additives.Add(EAdditiveType::Split);

	FRecipeOutput Output = RecipeSystem->EvaluateRecipe(Input);

	// 验证分裂效果
	TestTrue(TEXT("Should have SplitCount effect"), Output.Effects.Contains(RecipeEffectKeys::SplitCount));
	float SplitCount = Output.Effects[RecipeEffectKeys::SplitCount];
	TestEqual(TEXT("SplitCount should be 3"), SplitCount, 3.0f);

	// 验证稀有度
	TestEqual(TEXT("Rarity should be 1"), Output.Rarity, 1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecipeSystemMultiAdditiveTest, "EchoAlchemist.Alchemy.RecipeSystem.MultiAdditive", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecipeSystemMultiAdditiveTest::RunTest(const FString& Parameters)
{
	URecipeSystem* RecipeSystem = NewObject<URecipeSystem>();
	RecipeSystem->InitializeRecipes();

	// 测试多添加剂配方
	FRecipeInput Input;
	Input.BasePotion = EBasePotionType::Ricochet;
	Input.Additives.Add(EAdditiveType::Split);
	Input.Additives.Add(EAdditiveType::Fire);

	FRecipeOutput Output = RecipeSystem->EvaluateRecipe(Input);

	// 验证分裂效果
	TestTrue(TEXT("Should have SplitCount effect"), Output.Effects.Contains(RecipeEffectKeys::SplitCount));

	// 验证火焰伤害
	TestTrue(TEXT("Should have FireDamage effect"), Output.Effects.Contains(RecipeEffectKeys::FireDamage));

	// 验证稀有度
	TestTrue(TEXT("Rarity should be >= 2"), Output.Rarity >= 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecipeSystemDiscoveryTest, "EchoAlchemist.Alchemy.RecipeSystem.Discovery", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecipeSystemDiscoveryTest::RunTest(const FString& Parameters)
{
	URecipeSystem* RecipeSystem = NewObject<URecipeSystem>();
	RecipeSystem->InitializeRecipes();

	// 第一次评估配方
	FRecipeInput Input;
	Input.BasePotion = EBasePotionType::Ricochet;
	Input.Additives.Add(EAdditiveType::Split);

	FRecipeOutput Output1 = RecipeSystem->EvaluateRecipe(Input);
	TestTrue(TEXT("First evaluation should be new recipe"), Output1.bIsNewRecipe);

	// 第二次评估相同配方
	FRecipeOutput Output2 = RecipeSystem->EvaluateRecipe(Input);
	TestFalse(TEXT("Second evaluation should not be new recipe"), Output2.bIsNewRecipe);

	// 验证配方ID一致
	TestEqual(TEXT("Recipe IDs should match"), Output1.RecipeID, Output2.RecipeID);

	// 验证使用次数
	int32 UsageCount = RecipeSystem->GetRecipeUsageCount(Output1.RecipeID);
	TestEqual(TEXT("Usage count should be 2"), UsageCount, 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecipeSystemWorldStateTest, "EchoAlchemist.Alchemy.RecipeSystem.WorldState", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecipeSystemWorldStateTest::RunTest(const FString& Parameters)
{
	URecipeSystem* RecipeSystem = NewObject<URecipeSystem>();
	RecipeSystem->InitializeRecipes();

	// 测试世界状态影响
	FRecipeInput Input;
	Input.BasePotion = EBasePotionType::Ricochet;
	Input.Additives.Add(EAdditiveType::Fire);

	// 无世界状态
	FRecipeOutput Output1 = RecipeSystem->EvaluateRecipe(Input);
	float BaseDamage1 = Output1.Effects[RecipeEffectKeys::BaseDamage];

	// 添加高温世界状态
	Input.WorldState.Add(TEXT("Temperature"), 40.0f);
	FRecipeOutput Output2 = RecipeSystem->EvaluateRecipe(Input);
	float BaseDamage2 = Output2.Effects[RecipeEffectKeys::BaseDamage];

	// 验证效果倍率
	TestTrue(TEXT("EffectMultiplier should be > 1.0 in high temperature"), Output2.EffectMultiplier > 1.0f);
	TestTrue(TEXT("BaseDamage should be higher in high temperature"), BaseDamage2 > BaseDamage1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecipeSystemLibraryTest, "EchoAlchemist.Alchemy.RecipeSystem.Library", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecipeSystemLibraryTest::RunTest(const FString& Parameters)
{
	URecipeSystem* RecipeSystem = NewObject<URecipeSystem>();
	RecipeSystem->InitializeRecipes();

	// 评估几个配方
	FRecipeInput Input1;
	Input1.BasePotion = EBasePotionType::Ricochet;
	RecipeSystem->EvaluateRecipe(Input1);

	FRecipeInput Input2;
	Input2.BasePotion = EBasePotionType::Piercing;
	Input2.Additives.Add(EAdditiveType::Fire);
	RecipeSystem->EvaluateRecipe(Input2);

	// 获取已发现的配方
	TArray<FString> DiscoveredRecipes = RecipeSystem->GetDiscoveredRecipes();
	TestEqual(TEXT("Should have 2 discovered recipes"), DiscoveredRecipes.Num(), 2);

	// 获取配方定义
	TArray<FRecipeDefinition> RecipeDefinitions = RecipeSystem->GetDiscoveredRecipeDefinitions();
	TestEqual(TEXT("Should have 2 recipe definitions"), RecipeDefinitions.Num(), 2);

	// 重置配方库
	RecipeSystem->ResetRecipeLibrary();
	TArray<FString> AfterReset = RecipeSystem->GetDiscoveredRecipes();
	TestEqual(TEXT("Should have 0 recipes after reset"), AfterReset.Num(), 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecipeBlueprintLibraryTest, "EchoAlchemist.Alchemy.RecipeBlueprintLibrary.Basic", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecipeBlueprintLibraryTest::RunTest(const FString& Parameters)
{
	// 测试MakeRecipeInput
	TArray<EAdditiveType> Additives;
	Additives.Add(EAdditiveType::Split);
	Additives.Add(EAdditiveType::Fire);

	FRecipeInput Input = URecipeBlueprintLibrary::MakeRecipeInput(EBasePotionType::Ricochet, Additives);
	TestEqual(TEXT("BasePotion should be Ricochet"), Input.BasePotion, EBasePotionType::Ricochet);
	TestEqual(TEXT("Should have 2 additives"), Input.Additives.Num(), 2);

	// 测试AddWorldStateParameter
	URecipeBlueprintLibrary::AddWorldStateParameter(Input, TEXT("Temperature"), 35.0f);
	TestTrue(TEXT("Should have Temperature parameter"), Input.WorldState.Contains(TEXT("Temperature")));
	TestEqual(TEXT("Temperature should be 35.0"), Input.WorldState[TEXT("Temperature")], 35.0f);

	// 测试GetBasePotionDisplayName
	FString DisplayName = URecipeBlueprintLibrary::GetBasePotionDisplayName(EBasePotionType::Ricochet);
	TestEqual(TEXT("Display name should be correct"), DisplayName, TEXT("弹射药剂"));

	// 测试GetAdditiveDisplayName
	FString AdditiveName = URecipeBlueprintLibrary::GetAdditiveDisplayName(EAdditiveType::Fire);
	TestEqual(TEXT("Additive name should be correct"), AdditiveName, TEXT("火焰"));

	// 测试GetRarityColor
	FLinearColor Color = URecipeBlueprintLibrary::GetRarityColor(5);
	TestTrue(TEXT("Rarity 5 should have orange color"), Color.R > 0.5f && Color.G < 0.8f);

	// 测试GetRarityName
	FString RarityName = URecipeBlueprintLibrary::GetRarityName(5);
	TestEqual(TEXT("Rarity 5 should be 传说"), RarityName, TEXT("传说"));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecipeEffectExtractionTest, "EchoAlchemist.Alchemy.RecipeBlueprintLibrary.EffectExtraction", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRecipeEffectExtractionTest::RunTest(const FString& Parameters)
{
	// 创建测试配方输出
	FRecipeOutput Output;
	Output.Effects.Add(RecipeEffectKeys::BaseDamage, 10.0f);
	Output.Effects.Add(RecipeEffectKeys::FireDamage, 15.0f);

	// 测试GetEffectValue
	float BaseDamage = URecipeBlueprintLibrary::GetEffectValue(Output, RecipeEffectKeys::BaseDamage, 0.0f);
	TestEqual(TEXT("BaseDamage should be 10.0"), BaseDamage, 10.0f);

	float IceDamage = URecipeBlueprintLibrary::GetEffectValue(Output, RecipeEffectKeys::IceDamage, 99.0f);
	TestEqual(TEXT("IceDamage should use default value"), IceDamage, 99.0f);

	// 测试HasEffect
	TestTrue(TEXT("Should have BaseDamage effect"), URecipeBlueprintLibrary::HasEffect(Output, RecipeEffectKeys::BaseDamage));
	TestFalse(TEXT("Should not have IceDamage effect"), URecipeBlueprintLibrary::HasEffect(Output, RecipeEffectKeys::IceDamage));

	return true;
}
