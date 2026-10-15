// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"
#include "ABHpBarWidget.h"
#include "ABCharacterStatWidget.h"
#include <Interface/ABCharacterHUDInterface.h>

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UABHUDWidget::UpdateStat(
	const FABCharacterStat& InBaseStat, const FABCharacterStat& InModifierStat)
{
	// hpbar에 최대 체력 스탯 업데이트
	FABCharacterStat TotalStat = InBaseStat + InModifierStat;
	HpBar->UpdateStat(InBaseStat, InModifierStat);

	// 캐릭터 스탯 위젯에 새로운 데이터 전달
	CharacterStat->UpdateStat(InBaseStat, InModifierStat);
}

void UABHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	// Hp 상태 업데이트. 
	HpBar->UpdataHpBar(NewCurrentHp);
}

void UABHUDWidget::NativeConstruct() 
{
	Super::NativeConstruct();

	// HuD에서 이름으로 위젯 검색
	HpBar = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));

	ensureAlways(nullptr != HpBar);

	CharacterStat = Cast<UABCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));

	ensureAlways(nullptr != CharacterStat);

	// 캐릭터 스탯 컴포넌트 데이터 -> hud 스탯 위젯에 설정되어야함
	//GetOwningPlayer()->GetPawn() 폰가져오기
	//->  이 과정을 줄인 함수
	IABCharacterHUDInterface* HUDPawn = 
		GetOwningPlayerPawn<IABCharacterHUDInterface>();

	// hud위젯에 자신의 정보를 호출. 
	if (nullptr != HUDPawn)
		HUDPawn->SetupHUDWidget(this);

}
