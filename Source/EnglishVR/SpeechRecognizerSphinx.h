// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpeechRecognizer.h"
#include "pocketsphinx.h"
#include "Misc/Paths.h"
#include "SpeechRecognizerSphinx.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API USpeechRecognizerSphinx : public USpeechRecognizer {
    GENERATED_BODY()

public:
    USpeechRecognizerSphinx();
    virtual void Recognize(const FString& File) override;

private:
    ps_decoder_t* ps;

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
