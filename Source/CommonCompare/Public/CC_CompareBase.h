// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <CC_StructAndEnum.h>
#include "CC_CompareBase.generated.h"

/**
 * 比较基类
 */
UCLASS()
class COMMONCOMPARE_API UCC_CompareBase : public UObject
{
	GENERATED_BODY()
	
public:
	//比较结果
	UFUNCTION(BlueprintNativeEvent)
	bool CompareResult(FCC_BeCompareInfo BeCompareInfo, FCC_CompareInfo OuterTargetInfo);
	virtual bool CompareResult_Implementation(FCC_BeCompareInfo BeCompareInfo, FCC_CompareInfo OuterTargetInfo);
};
