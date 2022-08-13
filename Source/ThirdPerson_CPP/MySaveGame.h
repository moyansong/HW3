// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_CPP_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString PlayerName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        uint32 UserIndex;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        int64 UserScores;

    UMySaveGame();
};
