// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include <Player/ABPlayerController.h>
#include <Character/ABCharacterPlayer.h>

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacterPlayer::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();

	// 기본값 설정
	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AABGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	// 현재 점수를 전달받은 값으로 업데이트
	CurrentScore = NewPlayerScore;

	// 플레이어 컨트롤러에게 플레이어 점수가 업데이트 되었다는 것을 알라기
	// 기본저긍로 플레이어컨트롤러는 플레이어 수만큼 생성됨
	// 그러므로 싱글플레이어 게임이면 1개임
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if (nullptr != ABPlayerController)
	{
		// 겜 점수 변경 사실 알리기. 
		ABPlayerController->GameScoreChanged(CurrentScore);
	}

	// 게임 클리어 여부 확인
	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;
		if (nullptr != ABPlayerController)
		{
			ABPlayerController->GameClear();
		}
	}
}

void AABGameMode::OnPlayerDead()
{
	AABPlayerController* ABPlayerController = Cast<AABPlayerController>(GetWorld()->GetFirstPlayerController());
	if (nullptr != ABPlayerController)
	{
		ABPlayerController->GameOver();
	}
}

bool AABGameMode::IsGameCleared()
{
	return bIsCleared;
}
