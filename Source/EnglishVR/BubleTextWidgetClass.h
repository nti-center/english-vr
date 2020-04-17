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
        void SeeBotAnswer(TArray<FString> InputArray);

    UFUNCTION(BlueprintCallable)
        void SeeBotErrorAnswer(FString InputString);


};
