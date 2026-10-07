// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	MaxHP = 200.0f;
	SetHP(MaxHP);
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHP(MaxHP);
	
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	// 기존 HP 값 임시저장
	const float PrevHp = CurrentHP;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage); // 음수방지

	// 대미지 를 적용한 새 HP 계산(현재피 - 데미지값)
	SetHP(PrevHp - ActualDamage);

	// HP가 모두 소멸되었는지 확인. KINDA_SMALL_NUMBER: 0에 가까운 수
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		// 델리게이트 발행
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHP(float NewHP)
{
	// 현재 체력 업데이트
	CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, MaxHP);

	// 체력 변경 델리게이트 발행
	OnHpChanged.Broadcast(CurrentHP);
}
