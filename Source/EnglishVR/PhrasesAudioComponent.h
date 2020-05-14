// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Engine.h"
#include "BubleTextWidgetClass.h"
#include "Sound/SoundCue.h"
#include "EdGraph/EdGraph.h"
#include "Engine/DataTable.h"
#include "Components/WidgetComponent.h"
#include "CrossfadeDataTableStruct.h"
#include "Sound/SoundNodeWavePlayer.h"
#include "Sound/SoundNodeParamCrossFade.h"
#include "Sound/SoundNodeConcatenator.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Containers/Queue.h"
#include "PhrasesAudioComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENGLISHVR_API UPhrasesAudioComponent : public UAudioComponent {
    GENERATED_BODY()

public:
    UPhrasesAudioComponent();

    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TQueue<FString> SoundQueue;

    UPROPERTY(BlueprintReadWrite)
    USoundCue* SoundCue;

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle FuzeTimerHandle;

    UPROPERTY(BlueprintReadWrite)
    UBubleTextWidgetClass* WidgetBubble;

    UPROPERTY(EditDefaultsOnly)
    UDataTable* CrossfadeParametrsDataTable;

    UPROPERTY(BlueprintReadWrite)
    float TimerCount = 0;

    UPROPERTY(BlueprintReadWrite)
    float StepCount = 0;

    UPROPERTY(BlueprintReadWrite)
    float SummaryDuration = 0;

    UPROPERTY(BlueprintReadWrite)
    float CurrentDuration = 0;

    UPROPERTY(BlueprintReadWrite)
    TArray<float> SoundDuration;

    UPROPERTY(BlueprintReadWrite)
    TArray<USoundNodeWavePlayer*> PlayersArray;

    UFUNCTION(BlueprintCallable)
    void StartPlayingQueue();

    UFUNCTION(BlueprintCallable)
    int CreateCue(TArray<FString>InputArray, FString Name);

    UFUNCTION(BlueprintCallable)
    void PlaySoundWithCrossfade(TArray<FString> InputArray, UWidgetComponent* Widget, FString CharacterName);

    UFUNCTION(BlueprintCallable)
    void SetCrossfadeParametr();

private:
    UFUNCTION()
    void PlayNextSound();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
