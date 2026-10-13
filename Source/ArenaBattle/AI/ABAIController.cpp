// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ABAIController.h"
#include "ABAI.h"
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>

AABAIController::AABAIController()
{
	// 애셋 로드 및 설정
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(
		TEXT("/Game/ArenaBattle/AI/BB_ABCharater.BB_ABCharater")
	);

	if (BBAssetRef.Succeeded())
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(
		TEXT("/Game/ArenaBattle/AI/BT_ABCharacter.BT_ABCharacter")
	);

	if (BTAssetRef.Succeeded())
	{
		BTAsset = BTAssetRef.Object;
	}

}

void AABAIController::RunAI()
{
	// 블랙보드 컴포넌트 가져오기 (Blackboard와BlackboardPtr는 같은주솔가리킴 ) 
	UBlackboardComponent* BlackboardPtr = Blackboard;

	// 사용할 블랙보드 지정(L-Value로 가져와야해서 Blackboard인 채로 안하는거같고그럼)
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// NPC 폰이 생성된 위치를 블랙보드에 기록
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 비헤이비어트리 실행
		bool RunResult = RunBehaviorTree(BTAsset);
		// 실행 성공 여부 확인
		ensureAlways(RunResult);
	}
}

void AABAIController::StopAI()
{
	// 실행중인 bt 컴포넌트 가져오기
	UBehaviorTreeComponent* BTComponent =
		Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		// BT 실행 중지 처리
		BTComponent->StopTree();
	}
}

void AABAIController::OnPossess(APawn* InPawn)
{
	// 폰이 빙의하여 안전하게 getPawn을 사용할 수 있다. (RunAI에서 씀)
	Super::OnPossess(InPawn);

	// AI 실행
	RunAI();
}
