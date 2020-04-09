// Fill out your copyright notice in the Description page of Project Settings.


#include "PhrasesAudioComponent.h"

UPhrasesAudioComponent::UPhrasesAudioComponent() {
    PrimaryComponentTick.bCanEverTick = true;

    OnAudioFinished.AddDynamic(this, &UPhrasesAudioComponent::PlayNextSound);
}

void UPhrasesAudioComponent::BeginPlay() {
    Super::BeginPlay();
}

void UPhrasesAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPhrasesAudioComponent::StartPlayingQueue() {
    PlayNextSound();
}

void UPhrasesAudioComponent::PlayNextSound() {
    if (SoundQueue.IsEmpty())
        return;

    FString SoundName;
    SoundQueue.Dequeue(SoundName);

    FString Path = TEXT("/Game/Sounds/All_phrases/");
    FName FullSoundName = *(TEXT("SoundCue'") + Path + FString::Format(TEXT("{0}_Cue.{0}_Cue'"), { SoundName }));

    if (FullSoundName == NAME_None) {
        UE_LOG(LogTemp, Warning, TEXT("Incorrect sound name: %s"), *FullSoundName.ToString());
        return;
    }

    USoundCue* SoundCue = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, *FullSoundName.ToString()));
    if (!SoundCue) {
        UE_LOG(LogTemp, Warning, TEXT("Cant load sound cue: %s"), *FullSoundName.ToString());
        return;
    }

    SetSound(SoundCue);
    Play();
}