// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "SpeechRecognizer.generated.h"


UCLASS(Abstract)
class ENGLISHVR_API ASpeechRecognizer : public AActor {
    GENERATED_BODY()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRecognizedDelegate, FString, Text);

public:
    ASpeechRecognizer();

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FRecognizedDelegate OnRecognized;

    virtual void Recognize(const FString& File) PURE_VIRTUAL(ASpeechRecognizer::Recognize, return;);
    virtual void StopRecognition() PURE_VIRTUAL(ASpeechRecognizer::Recognize, return;);
    virtual void StartRecognition() PURE_VIRTUAL(ASpeechRecognizer::Recognize, return;);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
