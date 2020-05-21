// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"

#include "HandMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"

void UStartMenuWidget::NativeConstruct() {
    Super::NativeConstruct();

}

bool UStartMenuWidget::Initialize() {
    bool success = Super::Initialize();
    if (!success)  return false;

    if (!ensure(Button0_1 != nullptr)) return false;
    Button0_1->OnClicked.AddDynamic(this, &UStartMenuWidget::Button0_1Clicked);

    if (!ensure(Button0_2 != nullptr)) return false;
    Button0_2->OnClicked.AddDynamic(this, &UStartMenuWidget::Button0_2Clicked);

    //if (!ensure(Button1_1 != nullptr)) return false;
    //Button1_1->OnClicked.AddDynamic(this, &UStartMenuWidget::Button1_1Clicked);

    if (!ensure(Button1_3 != nullptr)) return false;
    Button1_3->OnClicked.AddDynamic(this, &UStartMenuWidget::Button1_3Clicked);

    return true;
}

void UStartMenuWidget::Button0_1Clicked() {
    UE_LOG(LogTemp, Error, TEXT("Button presed"));
    ChangeCanvas(Panel1, Panel2);
}

void UStartMenuWidget::Button0_2Clicked() {
    UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}

void UStartMenuWidget::Button1_1Clicked() {
    LoadLevel("Main");
}

void UStartMenuWidget::Button1_3Clicked() {
    ChangeCanvas(Panel2, Panel1);
}

void UStartMenuWidget::ChangeCanvas(UCanvasPanel* A, UCanvasPanel* B) {
    A->SetRenderOpacity(0);
    A->SetVisibility(ESlateVisibility::Hidden);

    B->SetRenderOpacity(1);
    B->SetVisibility(ESlateVisibility::Visible);
}

void UStartMenuWidget::LoadLevel(FString LevelName) {
    FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LevelName == CurrentLevel)
        return;
    //&IXRLoadingScreen::ShowLoadingScreen;
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true);

}