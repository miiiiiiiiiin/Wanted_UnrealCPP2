// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterControlData.h"
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 맵(TMap) 설정
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(
		TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder")
	);

	if (ShoulderDataRef.Succeeded())
	{
		CharacterControlManager.Add(
			ECharacterControlType::Shoulder,
			ShoulderDataRef.Object
		);
	}

	// 맵(TMap) 설정
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(
		TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater")
	);

	if (QuaterDataRef.Succeeded())
	{
		CharacterControlManager.Add(
			ECharacterControlType::Quater,
			QuaterDataRef.Object
		);
	}

}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* InCharacterControlData)
{
	// 데이터에서 속성을 가져와서 필요한 곳에 설정
	/*
	uint32 bUseControllerRotationYaw : 1;

	// 캐릭터 무브먼트에 설정할 회전 관련 속성.
	// 캐릭터 이동 시 컨트롤러와 같은 방향으로 움직일 것인지.
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseOrientToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 RotationRate;*/
	// 
	// Pawn 설정
	bUseControllerRotationYaw = InCharacterControlData->bUseControllerRotationYaw;

	// 캐릭터 무브먼트 설정
	GetCharacterMovement()->bUseControllerDesiredRotation
		= InCharacterControlData->bUseControllerDesiredRotation;

	GetCharacterMovement()->bOrientRotationToMovement
		= InCharacterControlData->bUseOrientToMovement;

	GetCharacterMovement()->RotationRate
		= InCharacterControlData->RotationRate;

}