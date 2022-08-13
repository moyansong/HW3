// Fill out your copyright notice in the Description page of Project Settings.

#include "UMGPlayerController.h"
#include "ThirdPerson_CPP.h"

void AUMGPlayerController::BeginPlay()
{
    Super::BeginPlay();
    //输入可以同时作用到UI和游戏里
    //SetInputMode(FInputModeGameAndUI());
}