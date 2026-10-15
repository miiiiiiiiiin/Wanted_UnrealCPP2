// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include <GameData/ABCharacterStat.h>
#include "ABScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABScrollItemData : public UABItemData
{
	GENERATED_BODY()
public: 
	UABScrollItemData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName()); // 에디터에 설정한 값
	}

public:
	// 기본 캐릭터 스탯 데이터
	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat BaseStat;
};
