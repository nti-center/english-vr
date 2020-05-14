// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FruitCostWidget.generated.h"


UCLASS()
class UFruitCostWidget : public UUserWidget {
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class USizeBox* FruitCostTextSizeBox = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* FruitCostText = nullptr;

    UFUNCTION(BlueprintCallable)
    void SetText(FString text);
};
