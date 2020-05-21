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
    class UCanvasPanel* Panel1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button0_1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button0Text_1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button0_2 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button0Text_2 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UCanvasPanel* Panel2 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button1_1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button1Text_1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button1_2 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button1Text_2 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button1_3 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button1Text_3 = nullptr;


    UFUNCTION(BlueprintCallable)
    void Button0_1Clicked();

    UFUNCTION(BlueprintCallable)
    void Button0_2Clicked();

    UFUNCTION(BlueprintCallable)
    void Button1_1Clicked();

    UFUNCTION(BlueprintCallable)
    void Button1_3Clicked();

    UFUNCTION(BlueprintCallable)
    void LoadLevel(FString LevelName);

    UFUNCTION(BlueprintCallable)
    void ChangeCanvas(UCanvasPanel* A, UCanvasPanel* B);

protected:
    virtual bool Initialize();


};
