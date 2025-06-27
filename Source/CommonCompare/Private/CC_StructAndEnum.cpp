#include <CC_StructAndEnum.h>
#include "CC_CompareBase.h"

bool FCC_BeCompareInfo::CompareResult(FCC_CompareInfo& OuterCompareInfo, FText& FailText)
{
	if (IsUseOuterCompareClass)
	{
		if (!OuterCompareClass.IsNull())
		{
			UCC_CompareBase* CC_CompareBase = NewObject<UCC_CompareBase>(GWorld, OuterCompareClass.LoadSynchronous());//这里可以作为对象池进行优化
			if (CC_CompareBase)
			{
				return CC_CompareBase->CompareResult(*this, OuterCompareInfo);
			}
		}
		return false;
	}
	else
	{
		//Tag对照
		if (!CompareTag_AllExact.IsEmpty())
		{
			for (const FGameplayTag& OtherTag : CompareTag_AllExact.GetGameplayTagArray())
			{
				if (!OuterCompareInfo.CompareTag.GetGameplayTagArray().Contains(OtherTag))//外部是不是每个Tag都包含
				{
					FailText = CompareFailText.GetFailText_TagAllExact(OtherTag);
					return false;
				}
			}
		}
		if (!CompareTag_AnyExact.IsEmpty() && !OuterCompareInfo.CompareTag.HasAnyExact(CompareTag_AnyExact))
		{
			FailText = CompareFailText.GetFailText_TagAnyExact();
			return false;
		}
		if (!CompareTag_All.IsEmpty() && !OuterCompareInfo.CompareTag.HasAll(CompareTag_All))
		{
			FailText = CompareFailText.GetFailText_TagAll();
			return false;
		}
		if (!CompareTag_Any.IsEmpty() && !OuterCompareInfo.CompareTag.HasAny(CompareTag_Any))
		{
			FailText = CompareFailText.GetFailText_TagAny();
			return false;
		}
		//Class对照
		if (!CompareClass.IsNull() && CompareClass != OuterCompareInfo.CompareClass && !(OuterCompareInfo.CompareObject && OuterCompareInfo.CompareObject->IsA(CompareClass.Get())))
		{
			FailText = CompareFailText.GetFailText_Class();
			return false;
		}
		//String对照
		if (!CompareString.IsEmpty() && CompareString != OuterCompareInfo.CompareString)
		{
			FailText = CompareFailText.GetFailText_String();
			return false;
		}
		//Float对照
		if (!CompareFloat.Contains(OuterCompareInfo.CompareFloat))
		{
			FailText = CompareFailText.GetFailText_Float();
			return false;
		}
	}
	return true;
}


