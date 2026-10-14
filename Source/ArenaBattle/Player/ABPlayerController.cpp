// Fill out your copyright notice in the Description page of Project Settings.

#include <UI/ABHUDWidget.h>
#include "Player/ABPlayerController.h"

AABPlayerController::AABPlayerController()
{
	// 위젯 블루프린트 애셋에서 클래스 정보 바인딩
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(
		TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C")
	);

	// 로드에 성공하면 클래스 설정
	if (ABHUDWidgetRef.Succeeded())
	{
		ABHUDwidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력설정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// 위젯 생성(소유컨트롤러, 위젯클래스 
	ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDwidgetClass);

	// 문제없이 생성되었다면 화면에 추가
		if (nullptr != ABHUDWidget)
		{
			ABHUDWidget->AddToViewport();
		}

}
