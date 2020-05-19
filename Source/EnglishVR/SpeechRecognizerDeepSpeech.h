// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpeechRecognizer.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "Networking.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "SocketSubsystem.h"
#include "portaudio.h"  
#include "SpeechRecognizerDeepSpeech.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API ASpeechRecognizerDeepSpeech : public ASpeechRecognizer {
    GENERATED_BODY()

public:
    ASpeechRecognizerDeepSpeech();
    virtual void Recognize(const FString& File) override;
    void ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
    FHttpModule* Http;
    FSocket* Socket;
    TSharedPtr<FInternetAddr> Address;
    FTimerHandle RecieveTimer;

    void RecieveData();
    void InitSocket();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;
};
