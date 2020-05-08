// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpeechRecognizer.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "SpeechRecognizerDeepSpeech.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API USpeechRecognizerDeepSpeech : public USpeechRecognizer {
    GENERATED_BODY()

public:
    USpeechRecognizerDeepSpeech();
    virtual void Recognize(const FString& File) override;
    void ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
    FHttpModule* Http;

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
