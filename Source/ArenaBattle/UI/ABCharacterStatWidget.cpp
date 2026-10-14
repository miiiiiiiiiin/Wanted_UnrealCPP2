// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include <Components/TextBlock.h>

void UABCharacterStatWidget::UpdateStat
(const FABCharacterStat& InBaseStat, const FABCharacterStat& InModifierStat)
{
	// FABCharacterStat이 가지는 속성을 리플랙션을 통해서 가져와 활용
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct());
		PropIt;
		++PropIt)
	{
		// 키로 사용할 속성 이름 값 저장
		const FName PropName(PropIt->GetName());

		// Base 스탯 데이터 설정을 위한 변수
		float BaseData = 0.0f;

		// 리플랙션을 활용해서 InBaseStat으로부터 속성 값 읽어오기. 
		// GetValue_InContainer:FProperty 값 가져오는 함수.. 
		PropIt->GetValue_InContainer(reinterpret_cast<const void*>(&InBaseStat), &BaseData);

		// 리플랙션을 활용해서 InModifierStat으로부터 속성 값 읽어오기
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer(reinterpret_cast<const void*>(&InModifierStat), &ModifierData);

		// 텍스트 블록에 값 설정. find가 포인터를 반환하니까 포인터담으려면 2차포인터 ㄱㄱ
		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropName);
		if (nullptr != BaseTextBlockPtr)
		{
			// float를 fstring으로 변환한 후 FString을 FText로 변환. 
			FText TextValue = FText::FromString(FString::SanitizeFloat(BaseData));
			// 텍스트 블록에 텍스트 값 설정. 
			(*BaseTextBlockPtr)->SetText(TextValue);
		}

		// 
		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropName);
		if (nullptr != ModifierTextBlockPtr)
		{
			// float를 fstring으로 변환한 후 FString을 FText로 변환. 
			FText TextValue = FText::FromString(FString::SanitizeFloat(ModifierData));
			// 텍스트 블록에 텍스트 값 설정. 
			(*ModifierTextBlockPtr)->SetText(TextValue);
		}
	}
}

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// FABCharacterStat이 가지는 속성을 리플랙션을 통해서 가져와 활용
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); 
		PropIt;
		++PropIt)
	{
		// 속성의 이름 값 (키)
		const FName PropName(PropIt->GetName());

		// Base 스탯에 대한 텍스트 블록 이름
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), 
			*PropIt->GetName());

		// Modifier 스탯에 대한 텍스트 블록 이름
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"),
			*PropIt->GetName());


		// 이름 값을 활용해 스탯 위젯에 있는 항목 가져오기
		UTextBlock* BaseTextBlock =
			Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));

		if (nullptr != BaseTextBlock)
		{
			// 검색한 텍스트 블록을 맵에 추가
			BaseLookup.Add(PropName, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock =
			Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (nullptr != ModifierTextBlock)
		{
			ModifierLookup.Add(PropName, ModifierTextBlock);
		}
	}
}
