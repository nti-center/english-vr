// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "speechapi_cxx.h"
#include "SpeechRecognitionMS.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API USpeechRecognitionMS : public USceneComponent {
    GENERATED_BODY()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRecognizedDelegate, FString, Text, int, Reason);

public:
    USpeechRecognitionMS();

    UFUNCTION(BlueprintCallable)
    void StopRecognition();

    UFUNCTION(BlueprintCallable)
    void StartRecognition();

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FRecognizedDelegate OnRecognized;

protected:
    virtual void BeginPlay() override;
    void Recognized(const Microsoft::CognitiveServices::Speech::SpeechRecognitionEventArgs& E);

    std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognizer> Recognizer;    

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
