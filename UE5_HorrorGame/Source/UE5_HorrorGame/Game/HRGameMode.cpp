// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HRGameMode.h"
#include "Player/HRPlayerController.h"
#include "Character/HRCharacterPlayer.h"

AHRGameMode::AHRGameMode()
{
	// cpp class : use StaticClass()
	// bp class : use FClassFinder<>

	// except for the metadatas below, using default ( by Unreal )

	//DefaultPawnClass = AHRCharacterPlayer::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Player/BP_CharacterPlayer.BP_CharacterPlayer_C"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	PlayerControllerClass = AHRPlayerController::StaticClass();		
	
}
