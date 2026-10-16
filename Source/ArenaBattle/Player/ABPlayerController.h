// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"
// 전방선언
class UABHUDWidget;
/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AABPlayerController();

	// 게임 처리와 관련된 함수. 
	// 공개 메소드 - 메시지
	// 객체지향 설계에서 객체 간의 소통은 메시지를 통해서...
	// 메소드(방법) - 메시지로 요청된 작업을 실제로 처리하는 방법. (로직)
	void GameScoreChanged(int32_t NewScore);
	void GameClear();
	void GameOver();

public:
	// 블프애서 구현 가능한 특별한 함수로 선언
	// K2접두어: Kismet(블루프린트 예전 버전임)
	// BlueprintImplementableEvent는 Cpp에서 구현 배치 안함.
	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);

	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();
private:
	virtual void BeginPlay() override;

protected:
	// 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<UABHUDWidget> ABHUDwidgetClass;

	// 생성한 위젯을 저장할 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<UABHUDWidget> ABHUDWidget;
};
