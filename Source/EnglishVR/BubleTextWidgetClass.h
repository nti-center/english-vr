// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Math/Color.h"
#include "Engine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "BubleTextWidgetClass.generated.h"


UCLASS()
class UBubleTextWidgetClass : public UUserWidget {
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class USizeBox* BubbleImageSizeBox = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class USizeBox* BubbleTextSizeBox = nullptr;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* BubbleText = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* BubbleImage = nullptr;

    UPROPERTY()
    UWorld* World;

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle FuzeTimerHandle;

    UFUNCTION(BlueprintCallable)
    void HideWidget();

    UFUNCTION(BlueprintCallable)
    FVector2D SetWidgetSize(FVector2D IncreaseImageSize, FVector2D IncreaseTextSize);

    UFUNCTION(BlueprintCallable)
    FVector2D SeeBotAnswer(TArray<FString> InputArray, int32 ErrorIndex);

    UFUNCTION(BlueprintCallable)
    FRotator MyLookRotation(FVector LookingActor, FVector TargetPosition, FVector WorldUp);

    UFUNCTION(BlueprintCallable)
    void SaveErrorToFile(FString word);

};
