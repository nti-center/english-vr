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

void UBubleTextWidgetClass::SeeBotAnswer(TArray<FString> InputArray) {
    FString TmpString;

    for (auto& name : InputArray) {
        TmpString += name + " ";
    }

    BubleImage->SetOpacity(1);
    BubleText->SetText(FText::FromString(TmpString));

    World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UBubleTextWidgetClass::HideWidget, 2.0f);
}

void UBubleTextWidgetClass::SeeBotErrorAnswer(FString InputString) {
    BubleImage->SetOpacity(1);
    BubleText->SetText(FText::FromString("Not Found sound: " + InputString));

    World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UBubleTextWidgetClass::HideWidget, 2.0f);
}


