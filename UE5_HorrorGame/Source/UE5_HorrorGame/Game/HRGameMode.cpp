// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HRGameMode.h"
#include "Player/HRPlayerController.h"
#include "Character/HRCharacterPlayer.h"

AHRGameMode::AHRGameMode()
{
	// cpp class : use StaticClass()
	// bp class : use FClassFinder<>

	// except for the metadatas below, using default ( by Unreal )

	// in editor, i use BP_HRGameMode
	// Default Pawn Class
	DefaultPawnClass = AHRCharacterPlayer::StaticClass();	// class instance

	// Player Controller class
	PlayerControllerClass = AHRPlayerController::StaticClass();		
	

}
