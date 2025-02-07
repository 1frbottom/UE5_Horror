// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HRGameMode.h"
#include "Player/HRPlayerController.h"
#include "Character/HRCharacterPlayer.h"

AHRGameMode::AHRGameMode()
{
	// cpp class : use StaticClass()
	// bp class : use FClassFinder<>

	// except for the metadatas below, using default ( by Unreal )

	// Default Pawn Class ( 잠깐 주석처리했음 )
	DefaultPawnClass = AHRCharacterPlayer::StaticClass();	// class instance

	// Player Controller class
	PlayerControllerClass = AHRPlayerController::StaticClass();		
	

}
