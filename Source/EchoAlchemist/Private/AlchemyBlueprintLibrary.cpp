// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemyBlueprintLibrary.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void UAlchemyBlueprintLibrary::TestAlchemySystem(UObject* WorldContextObject)
{
	// 1. 验证参数
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Test failed: Invalid World Context Object"));
		return;
	}
	
	// 2. 获取 World
	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Test failed: Invalid World"));
		return;
	}
	
	// 3. 获取 Game Instance
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Test failed: Invalid Game Instance"));
		return;
	}
	
	// 4. 获取炼金术系统
	UAlchemySystem* AlchemySystem = GameInstance->GetSubsystem<UAlchemySystem>();
	if (!AlchemySystem)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Test failed: Alchemy System not found"));
		return;
	}
	
	// 5. 运行测试
	UE_LOG(LogTemp, Log, TEXT("========================================"));
	UE_LOG(LogTemp, Log, TEXT("=== 炼金术系统测试开始 ==="));
	UE_LOG(LogTemp, Log, TEXT("========================================"));
	
	// 测试 1: Fire + Water = Steam
	FElement Result;
	bool bSuccess = AlchemySystem->CombineElements(TEXT("Fire"), TEXT("Water"), Result);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("✅ 测试 1 通过: Fire + Water = %s (Type: %s, Power: %d)"), 
			*Result.Name, *Result.Type, Result.Power);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 测试 1 失败: 无法合成 Fire + Water"));
	}
	
	// 测试 2: Earth + Water = Mud
	bSuccess = AlchemySystem->CombineElements(TEXT("Earth"), TEXT("Water"), Result);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("✅ 测试 2 通过: Earth + Water = %s (Type: %s, Power: %d)"), 
			*Result.Name, *Result.Type, Result.Power);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 测试 2 失败: 无法合成 Earth + Water"));
	}
	
	// 测试 3: Fire + Earth = Lava
	bSuccess = AlchemySystem->CombineElements(TEXT("Fire"), TEXT("Earth"), Result);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("✅ 测试 3 通过: Fire + Earth = %s (Type: %s, Power: %d)"), 
			*Result.Name, *Result.Type, Result.Power);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 测试 3 失败: 无法合成 Fire + Earth"));
	}
	
	// 测试 4: Air + Water = Cloud
	bSuccess = AlchemySystem->CombineElements(TEXT("Air"), TEXT("Water"), Result);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("✅ 测试 4 通过: Air + Water = %s (Type: %s, Power: %d)"), 
			*Result.Name, *Result.Type, Result.Power);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 测试 4 失败: 无法合成 Air + Water"));
	}
	
	// 测试 5: 无效配方 (Fire + Fire)
	bSuccess = AlchemySystem->CombineElements(TEXT("Fire"), TEXT("Fire"), Result);
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("✅ 测试 5 通过: Fire + Fire 正确返回失败（无配方）"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ 测试 5 异常: Fire + Fire 不应该有配方"));
	}
	
	UE_LOG(LogTemp, Log, TEXT("========================================"));
	UE_LOG(LogTemp, Log, TEXT("=== 炼金术系统测试结束 ==="));
	UE_LOG(LogTemp, Log, TEXT("========================================"));
}

FElement UAlchemyBlueprintLibrary::QuickCombine(UObject* WorldContextObject, const FString& Element1, const FString& Element2)
{
	FElement EmptyResult;
	
	// 获取炼金术系统（封装所有获取逻辑）
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UAlchemySystem* AlchemySystem = GameInstance ? GameInstance->GetSubsystem<UAlchemySystem>() : nullptr;
	
	if (!AlchemySystem)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Quick Combine failed: Alchemy System not found"));
		return EmptyResult;
	}
	
	// 执行合成并输出日志
	FElement Result;
	bool bSuccess = AlchemySystem->CombineElements(Element1, Element2, Result);
	
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("✅ %s + %s = %s (Type: %s, Power: %d)"), 
			*Element1, *Element2, *Result.Name, *Result.Type, Result.Power);
		return Result;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ %s + %s: 配方不存在"), *Element1, *Element2);
		return EmptyResult;
	}
}

TArray<FElement> UAlchemyBlueprintLibrary::GetAllElementsWithLog(UObject* WorldContextObject)
{
	TArray<FElement> EmptyArray;
	
	// 获取炼金术系统
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UAlchemySystem* AlchemySystem = GameInstance ? GameInstance->GetSubsystem<UAlchemySystem>() : nullptr;
	
	if (!AlchemySystem)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Get All Elements failed: Alchemy System not found"));
		return EmptyArray;
	}
	
	// 获取所有元素并输出日志
	TArray<FElement> Elements = AlchemySystem->GetAllElements();
	
	UE_LOG(LogTemp, Log, TEXT("========================================"));
	UE_LOG(LogTemp, Log, TEXT("=== 所有元素 (%d) ==="), Elements.Num());
	for (const FElement& Element : Elements)
	{
		UE_LOG(LogTemp, Log, TEXT("- %s (Type: %s, Power: %d)"), *Element.Name, *Element.Type, Element.Power);
	}
	UE_LOG(LogTemp, Log, TEXT("========================================"));
	
	return Elements;
}

TArray<FRecipe> UAlchemyBlueprintLibrary::GetAllRecipesWithLog(UObject* WorldContextObject)
{
	TArray<FRecipe> EmptyArray;
	
	// 获取炼金术系统
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UAlchemySystem* AlchemySystem = GameInstance ? GameInstance->GetSubsystem<UAlchemySystem>() : nullptr;
	
	if (!AlchemySystem)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Get All Recipes failed: Alchemy System not found"));
		return EmptyArray;
	}
	
	// 获取所有配方并输出日志
	TArray<FRecipe> Recipes = AlchemySystem->GetAllRecipes();
	
	UE_LOG(LogTemp, Log, TEXT("========================================"));
	UE_LOG(LogTemp, Log, TEXT("=== 所有配方 (%d) ==="), Recipes.Num());
	for (const FRecipe& Recipe : Recipes)
	{
		UE_LOG(LogTemp, Log, TEXT("- %s + %s = %s (Power: %d)"), 
			*Recipe.Element1, *Recipe.Element2, *Recipe.Result.Name, Recipe.Result.Power);
	}
	UE_LOG(LogTemp, Log, TEXT("========================================"));
	
	return Recipes;
}
