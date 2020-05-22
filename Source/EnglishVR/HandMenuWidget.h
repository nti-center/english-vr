// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "IXRLoadingScreen.h"
#include "HandMenuWidget.generated.h"

UCLASS()
class  UHandMenuWidget : public UUserWidget {
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UCanvasPanel* CanvasPanel1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button1Text = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button2 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button2Text = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button3 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button3Text = nullptr;



    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UCanvasPanel* CanvasPanel2 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* AnswerButton1 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* AnswerButton1Text = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* AnswerButton2 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* AnswerButton2Text = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* AnswerButton3 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* AnswerButton3Text = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* Button4 = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* Button4Text = nullptr;

   
    
    UFUNCTION(BlueprintCallable)
    void Button1Clicked();

    UFUNCTION(BlueprintCallable)
    void Button2Clicked();

    UFUNCTION(BlueprintCallable)
    void Button3Clicked();

    UFUNCTION(BlueprintCallable)
    void Button4Clicked();

    UFUNCTION(BlueprintCallable)
    void ChangeCanvas(UCanvasPanel* A, UCanvasPanel* B);

    UFUNCTION(BlueprintCallable)
    void LoadLevel(FString LevelName);


protected:
    virtual bool Initialize();

	
};
