// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "speechapi_cxx.h"
#include "SpeechRecognizer.h"
#include "SpeechRecognitionMS.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API USpeechRecognitionMS : public USpeechRecognizer {
    GENERATED_BODY()

public:
    USpeechRecognitionMS();

    UFUNCTION(BlueprintCallable)
    void StopRecognition();

    UFUNCTION(BlueprintCallable)
    void StartRecognition();

    virtual void Recognize(const FString& File) override;

protected:
    virtual void BeginPlay() override;
    void Recognized(const Microsoft::CognitiveServices::Speech::SpeechRecognitionEventArgs& E);
    void ParseResult(std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognitionResult> Result);

    std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognizer> Recognizer;    

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
