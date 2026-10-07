// Fill out your copyright notice in the Description page of Project Settings.


#include <Interface/ABAnitaionAttackInterface.h>
#include "Animation/AnimNotify_AttackHitCheck.h"

void UAnimNotify_AttackHitCheck::Notify(
	USkeletalMeshComponent* MeshComp, 
	UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference
)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 캐릭터에 접근해서 공격 판정 함수 호출
	if (MeshComp)
	{
		IABAnitaionAttackInterface* AttackPawn
			= Cast<IABAnitaionAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
