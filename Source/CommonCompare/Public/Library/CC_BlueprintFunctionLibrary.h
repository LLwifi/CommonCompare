// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <CC_StructAndEnum.h>
#include "CC_BlueprintFunctionLibrary.generated.h"


/**
 * 通用比对的蓝图函数库
 */
UCLASS()
class COMMONCOMPARE_API UCC_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//进行比较
	UFUNCTION(BlueprintCallable)
	static bool Compare(FCC_BeCompareInfo BeCompareInfo, FCC_CompareInfo OuterTargetInfo, FText& FailText);
};
