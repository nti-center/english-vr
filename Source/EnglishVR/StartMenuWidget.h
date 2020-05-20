// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "IXRLoadingScreen.h"
#include "StartMenuWidget.generated.h"

UCLASS()
class UStartMenuWidget : public UUserWidget {
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button1Text = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button2Text = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button2 = nullptr;


    UFUNCTION(BlueprintCallable)
    void Button1Clicked();

    UFUNCTION(BlueprintCallable)
    void Button2Clicked();

    UFUNCTION(BlueprintCallable)
    void LoadLevel(FString LevelName);

protected:
    virtual bool Initialize();


};
