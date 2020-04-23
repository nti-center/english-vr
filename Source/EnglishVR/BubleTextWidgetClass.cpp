// Fill out your copyright notice in the Description page of Project Settings.


#include "BubleTextWidgetClass.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ScaleBox.h"
#include "Components/Image.h"

void UBubleTextWidgetClass::NativeConstruct() {
    Super::NativeConstruct();

    World = GetWorld();
    HideWidget();
}

void UBubleTextWidgetClass::HideWidget() {
    if (BubbleText) {
        BubbleText->SetText(FText::FromString(FString(TEXT(""))));
    }

    if (BubbleImage) {
        BubbleImage->SetOpacity(0);
    }
}

void UBubleTextWidgetClass::SeeBotAnswer(TArray<FString> InputArray, int32 ErrorIndex) {
    FString TmpString;
    int32 Counter = 0;

   // if (InputArray.Num() > 2) {
   //     BubbleCanvas->SetRenderScale(FVector2D(2180.0f, 2560.0f));
   //     ImageScaleBox->SetRenderScale(FVector2D(2180.0f, 2560.0f));
   // }

    for (auto& name : InputArray) { 
        if (!((ErrorIndex > 0) && ((Counter+1) == ErrorIndex))) {
            name.ReplaceInline(TEXT("_"), TEXT(" "));
            if(Counter != 0)
                TmpString += name.ToLower() + " ";
            else {
                TmpString += name + " ";
                //TmpString += ", ";
            }
        }
        else {
            TmpString += name.ToUpper() + " ";
        }
        if((Counter+1) == InputArray.Num() && InputArray.Num() > 2)
            TmpString += "?";
        Counter++;
    }

    BubbleImage->SetOpacity(1);
    BubbleText->SetText(FText::FromString(TmpString));

    World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UBubleTextWidgetClass::HideWidget, 5.0f);
}

FRotator UBubleTextWidgetClass::MyLookRotation(FVector LookingActor, FVector TargetPosition, FVector WorldUp){
    FVector Forward = TargetPosition - LookingActor;
    FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(Forward, WorldUp);
    return Rot;
}




