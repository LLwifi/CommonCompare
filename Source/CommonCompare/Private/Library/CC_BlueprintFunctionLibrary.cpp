// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/CC_BlueprintFunctionLibrary.h"

bool UCC_BlueprintFunctionLibrary::Compare(FCC_BeCompareInfo BeCompareInfo, FCC_CompareInfo OuterTargetInfo, FText& FailText)
{
	return BeCompareInfo.CompareResult(OuterTargetInfo, FailText);
}
