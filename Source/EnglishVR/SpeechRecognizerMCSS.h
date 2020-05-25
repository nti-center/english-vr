// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "speechapi_cxx.h"
#include "SpeechRecognizer.h"
#include "Json.h"
#include "Misc/Paths.h"
#include "SpeechRecognizerMCSS.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API ASpeechRecognizerMCSS : public ASpeechRecognizer {
    GENERATED_BODY()

public:
    ASpeechRecognizerMCSS();

    virtual void StartRecognition() override;
    virtual void StopRecognition() override;
    virtual void Recognize(const FString& File) override;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void Recognized(const Microsoft::CognitiveServices::Speech::SpeechRecognitionEventArgs& E);
    void ParseResult(std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognitionResult> Result);

    std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognizer> Recognizer;    
    FString Subscription;
    FString Region;
    FString EndSilenceTimeoutMs;

public:
    virtual void Tick(float DeltaTime) override;
};
