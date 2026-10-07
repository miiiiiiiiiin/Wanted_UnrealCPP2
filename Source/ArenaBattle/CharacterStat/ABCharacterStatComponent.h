// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

// 델리게이트 선언
DECLARE_MULTICAST_DELEGATE(FOnHPZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChangedDelegate, float /*CurrentHP*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Getter
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHP; }

	// 대미지 적용 함수(setter)
	float ApplyDamage(float InDamage);

protected:
	// HP가 변경되었을 때 실행할 함수
	void SetHP(float NewHP);

public:
	// 체력을 모두 소진했을 때 발행할 델리게이트 선언
	FOnHPZeroDelegate OnHpZero;

	// 체력이 변경될 때 발행할 델리게이트 선언
	FOnHPChangedDelegate OnHpChanged;

protected:	
	// 최대 체력 값
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHP;

	// 현재 체력값
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float CurrentHP;
		
};
