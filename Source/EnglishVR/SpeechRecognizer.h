// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpeechRecognizer.generated.h"


UCLASS(Abstract)
class ENGLISHVR_API USpeechRecognizer : public USceneComponent {
    GENERATED_BODY()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRecognizedDelegate, FString, Text);

public:
    USpeechRecognizer();

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FRecognizedDelegate OnRecognized;

    virtual void Recognize(const FString& File) PURE_VIRTUAL(USpeechRecognizer::Recognize, return;);

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; 
};
