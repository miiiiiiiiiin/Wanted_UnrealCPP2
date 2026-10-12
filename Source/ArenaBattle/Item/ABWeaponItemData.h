// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "ABWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABWeaponItemData : public UABItemData
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName()); // 에디터에 설정한 값
	}
	// 제공할 무기에 대한 스켈레탈 메시 에셋
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<class USkeletalMesh> WeaponMesh;
	
};
