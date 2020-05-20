// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"

#include "HandMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"

void UStartMenuWidget::NativeConstruct() {
    Super::NativeConstruct();

}

bool UStartMenuWidget::Initialize() {
    bool success = Super::Initialize();
    if (!success)  return false;

    if (!ensure(Button1 != nullptr)) return false;
    Button1->OnClicked.AddDynamic(this, &UStartMenuWidget::Button1Clicked);

    if (!ensure(Button2 != nullptr)) return false;
    Button2->OnClicked.AddDynamic(this, &UStartMenuWidget::Button2Clicked);

    return true;
}

void UStartMenuWidget::Button1Clicked() {
    LoadLevel("Main");
}

void UStartMenuWidget::Button2Clicked() {
    UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}

void UStartMenuWidget::LoadLevel(FString LevelName) {
    FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LevelName == CurrentLevel)
        return;
    &IXRLoadingScreen::ShowLoadingScreen;
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true);

}