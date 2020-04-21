// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Math/Color.h"
#include "Engine.h"
#include "BubleTextWidgetClass.generated.h"


UCLASS()
class UBubleTextWidgetClass : public UUserWidget {
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UScaleBox* ImageScaleBox = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UScaleBox* TextScaleBox = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* BubleText = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* BubleImage = nullptr;

    UPROPERTY()
    UWorld* World;

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle FuzeTimerHandle;

    UFUNCTION(BlueprintCallable)
    void HideWidget();

    UFUNCTION(BlueprintCallable)
    void SeeBotAnswer(TArray<FString> InputArray, int32 ErrorIndex);

    UFUNCTION(BlueprintCallable)
    FRotator MyLookRotation(FVector lookAt, FVector upDirection, FVector Location);

};
