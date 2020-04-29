// Fill out your copyright notice in the Description page of Project Settings.


#include "BubleTextWidgetClass.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanelSlot.h"

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

FVector2D UBubleTextWidgetClass::SetWidgetSize(FVector2D IncreaseImageSize, FVector2D IncreaseTextSize) {
    FVector2D ImageSize = FVector2D(0.0f,0.0f);
    FVector2D TextSize = FVector2D(0.0f, 0.0f);

    UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(BubbleImageSizeBox->Slot);
    UCanvasPanelSlot* TextSlot = Cast<UCanvasPanelSlot>(BubbleTextSizeBox->Slot);

    if (!(ImageSlot && TextSlot))
        return FVector2D(0.0f, 0.0f);

    ImageSize = ImageSlot->GetSize();
    TextSize = TextSlot->GetSize();

    ImageSlot->SetSize(ImageSize + IncreaseImageSize);
    TextSlot->SetSize(TextSize + IncreaseTextSize);

    return FVector2D(ImageSize + IncreaseImageSize);
}

FVector2D UBubleTextWidgetClass::SeeBotAnswer(TArray<FString> InputArray, int32 ErrorIndex) {
    FString TmpString;
    int32 Counter = 0;
    FVector2D SetSize = FVector2D(0.0f, 0.0f);

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
            SaveErrorToFile(name);           
        }

        if((Counter+1) == InputArray.Num() && InputArray.Num() > 2)
            TmpString += "?";

        Counter++;
    }

    TArray<FString> CharacterArray = UKismetStringLibrary::GetCharacterArrayFromString(TmpString);
    //UE_LOG(LogTemp, Warning, TEXT("Num is: %d"), CharacterArray.Num());
    if (CharacterArray.Num() >= 60)
        SetSize = SetWidgetSize(FVector2D(640, 456), FVector2D(380, 240));

    BubbleImage->SetOpacity(1);
    BubbleText->SetText(FText::FromString(TmpString));

    World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UBubleTextWidgetClass::HideWidget, 2.5f);

    return SetSize;
}

FRotator UBubleTextWidgetClass::MyLookRotation(FVector LookingActor, FVector TargetPosition, FVector WorldUp) {
    FVector Forward = TargetPosition - LookingActor;
    FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(Forward, WorldUp);
    return Rot;
}

void UBubleTextWidgetClass::SaveErrorToFile(FString word) {
    FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir()) + TEXT("/ErrorSounds.txt");
    TArray<FString> TextArray;
    bool Check = false;
    FFileHelper::LoadFileToStringArray(TextArray, *FilePath);
    if (TextArray.Num() > 0) {
        for (auto& elem : TextArray) {
            if (elem == word) {
                Check = true;
            }
        }
    }
    if (Check == false) {
        FString FileContent = word + '\n';
        FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Already in array"));
    }
}




