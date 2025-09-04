// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStat/HRCharacterstat.h"
#include "HRGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHRGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class UE5_HORRORGAME_API UHRGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UHRGameSingleton();

	static UHRGameSingleton& Get();

// Character Stat Data Section
public:
	FORCEINLINE FHRCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FHRCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FHRCharacterStat> CharacterStatTable;

};
