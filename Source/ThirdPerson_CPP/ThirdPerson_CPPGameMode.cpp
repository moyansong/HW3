// Copyright Epic Games, Inc. All Rights Reserved.


#include "ThirdPerson_CPPGameMode.h"
#include "ThirdPerson_CPPCharacter.h"
#include "ThirdPerson_CPP.h"
#include "UObject/ConstructorHelpers.h"

AThirdPerson_CPPGameMode::AThirdPerson_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AThirdPerson_CPPGameMode::StartPlay()
{
	Super::StartPlay();
	//ChangeMenuWidget(StartingWidgetClass);
	//check(GEngine != nullptr);

	//// 显示调试消息五秒。 
	//// 参数中的-1"键"值类型参数能防止该消息被更新或刷新。
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameMode!"));

}

void AThirdPerson_CPPGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, int32 ZOrder)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport(ZOrder);
        }
    }
}