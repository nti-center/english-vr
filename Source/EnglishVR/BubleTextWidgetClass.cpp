// Fill out your copyright notice in the Description page of Project Settings.


#include "BubleTextWidgetClass.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UBubleTextWidgetClass::NativeConstruct() {
    Super::NativeConstruct();

    World = GetWorld();
    HideWidget();
}

void UBubleTextWidgetClass::HideWidget() {
    if (BubleText) {
        BubleText->SetText(FText::FromString(FString(TEXT(""))));
    }

    if (BubleImage) {
        BubleImage->SetOpacity(0);
    }
}

void UBubleTextWidgetClass::SeeBotAnswer(TArray<FString> InputArray, int32 ErrorIndex) {
    FString TmpString;
    int32 Counter = 0;

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

    BubleImage->SetOpacity(1);
    BubleText->SetText(FText::FromString(TmpString));

    World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UBubleTextWidgetClass::HideWidget, 5.0f);
}



