// Fill out your copyright notice in the Description page of Project Settings.


#include "FruitCostWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanelSlot.h"

void UFruitCostWidget::NativeConstruct() {
    Super::NativeConstruct();
}

void UFruitCostWidget::SetText(FString text){
    if (FruitCostText) {
        FruitCostText->SetText(FText::FromString(text));
    }
}
