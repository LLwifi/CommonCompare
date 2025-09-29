#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include <GameplayTagContainer.h>
#include "CC_StructAndEnum.generated.h"

class UCC_CompareBase;

/*对比对照结构体
* 该结构体将常见的比较情况进行了汇总
*/
USTRUCT(BlueprintType)
struct COMMONCOMPARE_API FCC_CompareInfo
{
	GENERATED_BODY()
public:
	FCC_CompareInfo Append(FCC_CompareInfo& CompareInfo)
	{
		CompareTag.AppendTags(CompareInfo.CompareTag);
		CompareClass.Append(CompareInfo.CompareClass);
		CompareString.Append(CompareInfo.CompareString);
		CompareFloat.Append(CompareInfo.CompareFloat);
		return *this;
	}

public:

	//对照Tag
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer CompareTag;

	/*对照Class
	* 和被比对（FCC_BeCompareInfo）信息比较时该Class = FCC_BeCompareInfo.CompareClass 或 CompareObject的类型为FCC_BeCompareInfo.CompareClass的子类
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UObject>> CompareClass;
	/*对照Obejct信息
	* 该值和被比对（FCC_BeCompareInfo）信息比较时该值的类型为FCC_BeCompareInfo.CompareClass的子类时可以通过calss的比对
	* 该值也可以用来判断一些更加细致的具体事项（例如复数信息），例如目标是否携带某种状态，交互的单位身价是否超过某个数值
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UObject*> CompareObject;

	//对照字符
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> CompareString;

	//对照数值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> CompareFloat;
};

/*对比失败的文本
*/
USTRUCT(BlueprintType)
struct COMMONCOMPARE_API FCC_CompareFailText
{
	GENERATED_BODY()
public:
	FText GetFailText_TagAllExact()
	{
		return IsUseUnifiedText ? FailText : CompareTag_AllExact_FailText;
	};
	FText GetFailText_TagAnyExact()
	{
		return IsUseUnifiedText? FailText: CompareTag_AnyExact_FailText;
	};
	FText GetFailText_TagAll()
	{
		return IsUseUnifiedText ? FailText : CompareTag_All_FailText;
	};
	FText GetFailText_TagAny()
	{
		return IsUseUnifiedText ? FailText : CompareTag_Any_FailText;
	};

	FText GetFailText_ClassAll()
	{
		return IsUseUnifiedText ? FailText : CompareClass_All_FailText;
	};
	FText GetFailText_ClassAny()
	{
		return IsUseUnifiedText ? FailText : CompareClass_Any_FailText;
	};
	FText GetFailText_ClassAllChild()
	{
		return IsUseUnifiedText ? FailText : CompareClass_AllChild_FailText;
	};
	FText GetFailText_ClassAnyChild()
	{
		return IsUseUnifiedText ? FailText : CompareClass_AnyChild_FailText;
	};

	FText GetFailText_StringAll()
	{
		return IsUseUnifiedText ? FailText : CompareString_All_FailText;
	};
	FText GetFailText_StringAny()
	{
		return IsUseUnifiedText ? FailText : CompareString_Any_FailText;
	};

	FText GetFailText_FloatAll()
	{
		return IsUseUnifiedText ? FailText : CompareFloat_All_FailText;
	};
	FText GetFailText_FloatAny()
	{
		return IsUseUnifiedText ? FailText : CompareFloat_Any_FailText;
	};
public:
	//是否使用统一文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsUseUnifiedText = false;
	//统一比对失败的文本/提示
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "IsUseUnifiedText"))
	FText FailText;

	/*Tag比对失败的文本。外部需要全部匹配，且Tag需要完全/精准匹配
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareTag_AllExact_FailText;

	//Tag比对失败的文本/提示。外部只用满足任意一个即可，且Tag需要完全/精准匹配
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareTag_AnyExact_FailText;

	/*Tag比对失败的文本。外部需要全部匹配，Tag包含父类即可
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareTag_All_FailText;

	//Tag比对失败的文本/提示。外部只用满足任意一个即可，Tag包含父类即可
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareTag_Any_FailText;

	//Class比对失败的文本/提示。外部需要全部匹配，class必须完全匹配
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareClass_All_FailText;
	//Class比对失败的文本/提示。外部只用满足任意一个即可，class必须完全匹配
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareClass_Any_FailText;
	//Class比对失败的文本/提示。外部需要全部匹配，class的判断包含子类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareClass_AllChild_FailText;
	//Class比对失败的文本/提示。外部只用满足任意一个即可，class的判断包含子类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareClass_AnyChild_FailText;

	//字符比对失败的文本/提示。外部需要全部匹配
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareString_All_FailText;
	//字符比对失败的文本/提示。外部只用满足任意一个即可
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareString_Any_FailText;

	//数值比对失败的文本/提示。外部需要全部匹配
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareFloat_All_FailText;
	//数值比对失败的文本/提示。外部只用满足任意一个即可
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseUnifiedText"))
	FText CompareFloat_Any_FailText;
};

/*被对比对照结构体
* 该结构体将常见的比较情况进行了汇总
*/
USTRUCT(BlueprintType)
struct COMMONCOMPARE_API FCC_BeCompareInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	bool CompareResult(FCC_CompareInfo& OuterCompareInfo, FText& FailText);

public:
	//比对方式是否使用外部比对类
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsUseOuterCompareClass = false;

	//外部对照类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "IsUseOuterCompareClass"))
	TSoftClassPtr<UCC_CompareBase> OuterCompareClass;

	/*对照Tag
	* 这里配置的Tag外部需要全部匹配，且Tag需要完全/精准匹配
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Compare"), meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	FGameplayTagContainer CompareTag_AllExact;

	/*对照Tag
	* 这里配置的Tag外部只用满足任意一个即可，且Tag需要完全/精准匹配
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Compare"), meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	FGameplayTagContainer CompareTag_AnyExact;

	/*对照Tag
	* 这里配置的Tag外部需要全部匹配，Tag包含父类即可
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Compare"), meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	FGameplayTagContainer CompareTag_All;

	/*对照Tag
	* 这里配置的Tag外部只用满足任意一个即可，Tag包含父类即可
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Compare"), meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	FGameplayTagContainer CompareTag_Any;

	/*对照Class 这里配置的class外部需要全部匹配，class必须完全匹配
	* 例如判断持有的道具是否是某个Class的子类
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	TArray<TSoftClassPtr<UObject>> CompareClass_All;
	/*对照Class 这里配置的class外部只用满足任意一个即可，class必须完全匹配
	* 例如判断持有的道具是否是某个Class的子类
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	TArray<TSoftClassPtr<UObject>> CompareClass_Any;
	/*对照Class 这里配置的class外部需要全部匹配，class的判断包含子类
	* 例如判断持有的道具是否是某个Class的子类
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	TArray<TSoftClassPtr<UObject>> CompareClass_AllChild;
	/*对照Class 这里配置的class外部只用满足任意一个即可，class的判断包含子类
	* 例如判断持有的道具是否是某个Class的子类
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	TArray<TSoftClassPtr<UObject>> CompareClass_AnyChild;

	/*对照字符比对
	* 某些不至于使用UObject但是Class不足以判断内容时，可以选择使用该内容进行判断
	* 例如：目标是否携带某种状态也可以通过定于该值进行判断：例如用Fire代表燃烧；Ice代表冰冻
	* 这里的配置外部需要全部匹配
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	TArray<FString> CompareString_All;
	/*对照字符比对
	* 某些不至于使用UObject但是Class不足以判断内容时，可以选择使用该内容进行判断
	* 例如：目标是否携带某种状态也可以通过定于该值进行判断：例如用Fire代表燃烧；Ice代表冰冻
	* 这里的配置外部只用满足任意一个即可
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	TArray<FString> CompareString_Any;

	/*数值对比
	* 这里的配置外部需要全部匹配
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	FFloatRange CompareFloat_All;
	/*数值对比
	* 这里的配置外部只用满足任意一个即可
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "!IsUseOuterCompareClass"))
	TArray<FFloatRange> CompareFloat_Any;

	/*对比失败的文本
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCC_CompareFailText CompareFailText;
};