// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void UWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D ScreenPosition;
	//将第二个参数世界位置投影到屏幕
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), ScreenPosition))
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}