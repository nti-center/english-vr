// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Containers/Queue.h"
#include "PhrasesAudioComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENGLISHVR_API UPhrasesAudioComponent : public UAudioComponent {
    GENERATED_BODY()

public:
    UPhrasesAudioComponent();

    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TQueue<FString> SoundQueue;

    UFUNCTION(BlueprintCallable)
    void StartPlayingQueue();

private:
    UFUNCTION()
    void PlayNextSound();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
