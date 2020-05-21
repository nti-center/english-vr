// Fill out your copyright notice in the Description page of Project Settings.


#include "HandMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"

void UHandMenuWidget::NativeConstruct() {
    Super::NativeConstruct();
}

bool UHandMenuWidget::Initialize() {
    bool success = Super::Initialize();
    if (!success)  return false;

   // if (!ensure(Button1 != nullptr)) return false;
   // Button1->OnClicked.AddDynamic(this, &UHandMenuWidget::Button1Clicked);

    if (!ensure(Button2 != nullptr)) return false;
    Button2->OnClicked.AddDynamic(this, &UHandMenuWidget::Button2Clicked);

    if (!ensure(Button3 != nullptr)) return false;
    Button3->OnClicked.AddDynamic(this, &UHandMenuWidget::Button3Clicked);
  
    if (!ensure(Button4 != nullptr)) return false;
    Button4->OnClicked.AddDynamic(this, &UHandMenuWidget::Button4Clicked);

    return true;
}

void UHandMenuWidget::Button1Clicked() {
    LoadLevel("StartMenu");
}

void UHandMenuWidget::Button2Clicked() {
    UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit,false);
}

void UHandMenuWidget::Button3Clicked() {
    ChangeCanvas(CanvasPanel1, CanvasPanel2);
}

void UHandMenuWidget::Button4Clicked() {
    ChangeCanvas(CanvasPanel2, CanvasPanel1);
}

void UHandMenuWidget::ChangeCanvas(UCanvasPanel* A, UCanvasPanel* B) {
    A->SetRenderOpacity(0);
    A->SetVisibility(ESlateVisibility::Hidden);

    B->SetRenderOpacity(1);
    B->SetVisibility(ESlateVisibility::Visible);
}

void UHandMenuWidget::LoadLevel(FString LevelName) {
    FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LevelName == CurrentLevel)
        return;
    //&IXRLoadingScreen::ShowLoadingScreen;
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true);

}
