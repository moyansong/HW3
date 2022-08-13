// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPerson_CPPGameMode.generated.h"

UCLASS(minimalapi)
class AThirdPerson_CPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThirdPerson_CPPGameMode();

private:
	virtual void StartPlay() override;

public:
    /** 移除当前菜单控件，并在指定类（如有）中新建控件。*/
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
        void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 ZOrder = 0);

protected:
    /** 游戏开始时调用。*/
    //virtual void BeginPlay() override;

    /** 游戏开始时，用作菜单的控件类。用一个模板是为了在游戏中随时更换CurrWidget*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

    /** 用作菜单的控件实例。*/
    UPROPERTY()
        UUserWidget* CurrentWidget;
};



