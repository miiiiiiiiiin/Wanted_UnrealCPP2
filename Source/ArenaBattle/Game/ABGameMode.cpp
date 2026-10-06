// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include <Player/ABPlayerController.h>
#include <Character/ABCharacterPlayer.h>

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacterPlayer::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();

}
