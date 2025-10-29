#include <CC_StructAndEnum.h>
#include "CC_CompareBase.h"

bool FCC_BeCompareCommonInfo::IsInvertReturn(bool IsInvert, bool CompareResult)
{
	if (IsInvert)
	{
		return !CompareResult;
	}
	return CompareResult;
}

bool FCC_BeCompareCommonInfo::CompareResult(FCC_CompareInfo& OuterCompareInfo, FText& FailText)
{
	bool b = false;
	//Tag对照
	if (!CompareTag_AllExact.IsEmpty())
	{
		b = true;
		for (const FGameplayTag& OtherTag : CompareTag_AllExact.GetGameplayTagArray())
		{
			if (!OuterCompareInfo.CompareTag.GetGameplayTagArray().Contains(OtherTag))//外部是不是每个Tag都包含
			{
				b = false;
				break;
			}
		}

		if (!IsInvertReturn(CompareTag_AllExact_IsInvert,b))
		{
			FailText = CompareFailText.GetFailText_TagAllExact();
			return false;
		}
	}

	b = true;
	if (!CompareTag_AnyExact.IsEmpty() && !OuterCompareInfo.CompareTag.HasAnyExact(CompareTag_AnyExact))
	{
		b = false;
	}
	if (!IsInvertReturn(CompareTag_AnyExact_IsInvert, b))
	{
		FailText = CompareFailText.GetFailText_TagAnyExact();
		return false;
	}

	b = true;
	if (!CompareTag_All.IsEmpty() && !OuterCompareInfo.CompareTag.HasAll(CompareTag_All))
	{
		b = false;
	}
	if (!IsInvertReturn(CompareTag_All_IsInvert, b))
	{
		FailText = CompareFailText.GetFailText_TagAll();
		return false;
	}

	b = true;
	if (!CompareTag_Any.IsEmpty() && !OuterCompareInfo.CompareTag.HasAny(CompareTag_Any))
	{
		b = false;
	}
	if (!IsInvertReturn(CompareTag_Any_IsInvert, b))
	{
		FailText = CompareFailText.GetFailText_TagAny();
		return false;
	}

	//Class对照
	TArray<TSubclassOf<UObject>> CompareClassArray = OuterCompareInfo.CompareClass;
	if (OuterCompareInfo.CompareObject.Num() > 0)//将外部的Object的class也进行储存
	{
		for (UObject*& obj : OuterCompareInfo.CompareObject)
		{
			CompareClassArray.Add(obj->GetClass());
		}
	}

	if (CompareClass_All.Num() > 0)
	{
		b = true;
		for (TSoftClassPtr<UObject>& CompareClass : CompareClass_All)
		{
			if (!CompareClassArray.Contains(CompareClass.LoadSynchronous()))//全部匹配，不允许有一个不包含
			{
				b = false;
				break;
			}
		}
		if (!IsInvertReturn(CompareClass_All_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_ClassAll();
			return false;
		}
	}
	if (CompareClass_Any.Num() > 0)
	{
		b = false;
		for (TSoftClassPtr<UObject>& CompareClass : CompareClass_Any)
		{
			if (CompareClassArray.Contains(CompareClass.LoadSynchronous()))//外部有任意一个即可
			{
				b = true;
				break;
			}
		}
		if (!IsInvertReturn(CompareClass_Any_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_ClassAny();
			return false;
		}
	}
	if (CompareClass_AllChild.Num() > 0)//包含子类的全部匹配，不允许有一个不包含
	{
		for (TSoftClassPtr<UObject>& CompareClass : CompareClass_AllChild)
		{
			b = false;
			for (TSubclassOf<UObject>& OutClass : CompareClassArray)
			{
				if (OutClass && OutClass->IsChildOf(CompareClass.LoadSynchronous()))
				{
					b = true;
					break;
				}
			}

			if (!IsInvertReturn(CompareClass_AllChild_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_ClassAllChild();
				return false;
			}
		}
	}
	if (CompareClass_AnyChild.Num() > 0)//包含子类的匹配，外部只用满足任意一个即可
	{
		b = false;
		for (TSoftClassPtr<UObject>& CompareClass : CompareClass_AnyChild)
		{
			for (TSubclassOf<UObject>& OutClass : CompareClassArray)
			{
				if (OutClass && OutClass->IsChildOf(CompareClass.LoadSynchronous()))
				{
					b = true;
					break;
				}
			}
			if (b)
			{
				break;
			}
		}
		if (!IsInvertReturn(CompareClass_AnyChild_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_ClassAnyChild();
			return false;
		}
	}
	//String对照
	if (CompareString_All.Num() > 0)
	{
		b = true;
		for (FString& s : CompareString_All)
		{
			if (!OuterCompareInfo.CompareString.Contains(s))//全部匹配，不允许有一个不包含
			{
				b = false;
			}
		}
		if (!IsInvertReturn(CompareString_All_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_StringAll();
			return false;
		}
	}
	if (CompareString_Any.Num() > 0)
	{
		b = false;
		for (FString& s : CompareString_Any)
		{
			if (OuterCompareInfo.CompareString.Contains(s))//外部有任意一个即可
			{
				b = true;
				break;
			}
		}
		if (!IsInvertReturn(CompareString_Any_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_StringAny();
			return false;
		}
	}

	//Float对照
	if (CompareFloat_All.HasLowerBound() || CompareFloat_All.HasUpperBound())
	{
		b = false;
		for (float& f : OuterCompareInfo.CompareFloat)
		{
			if (CompareFloat_All.Contains(f))
			{
				b = true;
				break;
			}
		}
		if (!IsInvertReturn(CompareFloat_All_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_FloatAll();
			return false;
		}
	}

	if (CompareFloat_Any.Num() > 0)
	{
		b = false;
		for (float& f : OuterCompareInfo.CompareFloat)
		{
			for (FFloatRange FloatRange : CompareFloat_Any)
			{
				if (FloatRange.Contains(f))
				{
					b = true;
					break;
				}
			}
			if (b)
			{
				break;
			}
		}
		if (!IsInvertReturn(CompareFloat_Any_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_FloatAny();
			return false;
		}
	}
	return true;
}

bool FCC_BeCompareInfo::IsInvertReturn(bool IsInvert, bool CompareResult)
{
	if (IsInvert)
	{
		return !CompareResult;
	}
	return CompareResult;
}

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
		//BeCompareCommonInfo.CompareResult(OuterCompareInfo, FailText);
		bool b = false;
		//Tag对照
		if (!CompareTag_AllExact.IsEmpty())
		{
			b = true;
			for (const FGameplayTag& OtherTag : CompareTag_AllExact.GetGameplayTagArray())
			{
				if (!OuterCompareInfo.CompareTag.GetGameplayTagArray().Contains(OtherTag))//外部是不是每个Tag都包含
				{
					b = false;
					break;
				}
			}

			if (!IsInvertReturn(CompareTag_AllExact_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_TagAllExact();
				return false;
			}
		}

		b = true;
		if (!CompareTag_AnyExact.IsEmpty() && !OuterCompareInfo.CompareTag.HasAnyExact(CompareTag_AnyExact))
		{
			b = false;
		}
		if (!IsInvertReturn(CompareTag_AnyExact_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_TagAnyExact();
			return false;
		}

		b = true;
		if (!CompareTag_All.IsEmpty() && !OuterCompareInfo.CompareTag.HasAll(CompareTag_All))
		{
			b = false;
		}
		if (!IsInvertReturn(CompareTag_All_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_TagAll();
			return false;
		}

		b = true;
		if (!CompareTag_Any.IsEmpty() && !OuterCompareInfo.CompareTag.HasAny(CompareTag_Any))
		{
			b = false;
		}
		if (!IsInvertReturn(CompareTag_Any_IsInvert, b))
		{
			FailText = CompareFailText.GetFailText_TagAny();
			return false;
		}

		//Class对照
		TArray<TSubclassOf<UObject>> CompareClassArray = OuterCompareInfo.CompareClass;
		if (OuterCompareInfo.CompareObject.Num() > 0)//将外部的Object的class也进行储存
		{
			for (UObject*& obj : OuterCompareInfo.CompareObject)
			{
				CompareClassArray.Add(obj->GetClass());
			}
		}

		if (CompareClass_All.Num() > 0)
		{
			b = true;
			for (TSoftClassPtr<UObject>& CompareClass : CompareClass_All)
			{
				if (!CompareClassArray.Contains(CompareClass.LoadSynchronous()))//全部匹配，不允许有一个不包含
				{
					b = false;
					break;
				}
			}
			if (!IsInvertReturn(CompareClass_All_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_ClassAll();
				return false;
			}
		}
		if (CompareClass_Any.Num() > 0)
		{
			b = false;
			for (TSoftClassPtr<UObject>& CompareClass : CompareClass_Any)
			{
				if (CompareClassArray.Contains(CompareClass.LoadSynchronous()))//外部有任意一个即可
				{
					b = true;
					break;
				}
			}
			if (!IsInvertReturn(CompareClass_Any_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_ClassAny();
				return false;
			}
		}
		if (CompareClass_AllChild.Num() > 0)//包含子类的全部匹配，不允许有一个不包含
		{
			for (TSoftClassPtr<UObject>& CompareClass : CompareClass_AllChild)
			{
				b = false;
				for (TSubclassOf<UObject>& OutClass : CompareClassArray)
				{
					if (OutClass && OutClass->IsChildOf(CompareClass.LoadSynchronous()))
					{
						b = true;
						break;
					}
				}

				if (!IsInvertReturn(CompareClass_AllChild_IsInvert, b))
				{
					FailText = CompareFailText.GetFailText_ClassAllChild();
					return false;
				}
			}
		}
		if (CompareClass_AnyChild.Num() > 0)//包含子类的匹配，外部只用满足任意一个即可
		{
			b = false;
			for (TSoftClassPtr<UObject>& CompareClass : CompareClass_AnyChild)
			{
				for (TSubclassOf<UObject>& OutClass : CompareClassArray)
				{
					if (OutClass && OutClass->IsChildOf(CompareClass.LoadSynchronous()))
					{
						b = true;
						break;
					}
				}
				if (b)
				{
					break;
				}
			}
			if (!IsInvertReturn(CompareClass_AnyChild_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_ClassAnyChild();
				return false;
			}
		}
		//String对照
		if (CompareString_All.Num() > 0)
		{
			b = true;
			for (FString& s : CompareString_All)
			{
				if (!OuterCompareInfo.CompareString.Contains(s))//全部匹配，不允许有一个不包含
				{
					b = false;
				}
			}
			if (!IsInvertReturn(CompareString_All_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_StringAll();
				return false;
			}
		}
		if (CompareString_Any.Num() > 0)
		{
			b = false;
			for (FString& s : CompareString_Any)
			{
				if (OuterCompareInfo.CompareString.Contains(s))//外部有任意一个即可
				{
					b = true;
					break;
				}
			}
			if (!IsInvertReturn(CompareString_Any_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_StringAny();
				return false;
			}
		}

		//Float对照
		if (CompareFloat_All.HasLowerBound() || CompareFloat_All.HasUpperBound())
		{
			b = false;
			for (float& f : OuterCompareInfo.CompareFloat)
			{
				if (CompareFloat_All.Contains(f))
				{
					b = true;
					break;
				}
			}
			if (!IsInvertReturn(CompareFloat_All_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_FloatAll();
				return false;
			}
		}

		if (CompareFloat_Any.Num() > 0)
		{
			b = false;
			for (float& f : OuterCompareInfo.CompareFloat)
			{
				for (FFloatRange FloatRange : CompareFloat_Any)
				{
					if (FloatRange.Contains(f))
					{
						b = true;
						break;
					}
				}
				if (b)
				{
					break;
				}
			}
			if (!IsInvertReturn(CompareFloat_Any_IsInvert, b))
			{
				FailText = CompareFailText.GetFailText_FloatAny();
				return false;
			}
		}
	}
	return true;
}


