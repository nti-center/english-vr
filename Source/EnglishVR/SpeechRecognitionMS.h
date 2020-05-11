// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "speechapi_cxx.h"
#include "SpeechRecognizer.h"
#include "SpeechRecognitionMS.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API ASpeechRecognitionMS : public ASpeechRecognizer {
    GENERATED_BODY()

public:
    ASpeechRecognitionMS();

    UFUNCTION(BlueprintCallable)
    void StopRecognition();

    UFUNCTION(BlueprintCallable)
    void StartRecognition();

    UFUNCTION(BlueprintCallable)
    void CreateReognizer(const FString& Subscription, const FString& Region);

    virtual void Recognize(const FString& File) override;

protected:
    virtual void BeginPlay() override;
    void Recognized(const Microsoft::CognitiveServices::Speech::SpeechRecognitionEventArgs& E);
    void ParseResult(std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognitionResult> Result);

    std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognizer> Recognizer;    

public:
    virtual void Tick(float DeltaTime) override;
};
