// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAI.h"
#include "AI/BTTask_FindPatrolPos.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <NavigationSystem.h>

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{

}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 상위 로직 호출 후 리턴값 가져오기
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    // 다음 정찰할 위치 선택 로직

    // 네비게이션 시스템을 활용하기 위한 포인터 얻기
    UNavigationSystemV1::GetNavigationSystem();

    // 폰이 생성된 초기 위치 가져오기
    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);

    // 네비게이션 기능 활용 - 이동 가능한 위치를 랜덤으로 선택



    return EBTNodeResult::Type();
}
