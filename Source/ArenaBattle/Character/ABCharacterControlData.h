// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UABCharacterControlData();

	// 속성
	// 
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	// 캐릭터 무브먼트에 설정할 회전 관련 속성.
	// 캐릭터 이동 시 컨트롤러와 같은 방향으로 움직일 것인지.
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseOrientToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	// 사용할 입력 매핑 컨텍스트 애셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// 스프링 암 설정
	// 스프링암이 물체와 닿았을 때 일시적으로 짧아지게 만들어서 줌인되게만드는옵션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	float TargetArmLength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bDoCollisionText: 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bUsePawnControlRotation: 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritPitch: 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritYaw: 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritRoll: 1;


};
