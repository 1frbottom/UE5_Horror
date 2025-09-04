// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HRGameSingleton.h"

DEFINE_LOG_CATEGORY(LogHRGameSingleton);

UHRGameSingleton::UHRGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/HRCharacterStatTable.HRCharacterStatTable'"));

	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);

		Algo::Transform
		(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FHRCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UHRGameSingleton& UHRGameSingleton::Get()
{
	UHRGameSingleton* Singleton = CastChecked<UHRGameSingleton>(GEngine->GameSingleton);

	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogHRGameSingleton, Error, TEXT("Invalid Game Singleton"));

	return *NewObject<UHRGameSingleton>();
}