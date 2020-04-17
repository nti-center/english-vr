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

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path) {
    if (Path == NAME_None) return NULL;
    return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

void UPhrasesAudioComponent::CreateCue(TArray<FString> InputArray) {
    TArray<struct FDistanceDatum> Datum;
    FString SoundCueName = "CueWithCrossfade";
    SoundCue = NewObject<USoundCue>(this, *SoundCueName);
    int32 NodeIndex = 0;

    USoundNodeParamCrossFade* Crossfade = SoundCue->ConstructSoundNode<USoundNodeParamCrossFade>();
    USoundNodeConcatenator* Concatenator = SoundCue->ConstructSoundNode<USoundNodeConcatenator>();

    Crossfade->ParamName = "CrossfadeParam";

    Crossfade->GraphNode->NodePosX = -130;
    Crossfade->GraphNode->NodePosY = 50 * InputArray.Num() / 2;

    // SoundCue->FirstNode = Crossfade;
    SoundCue->FirstNode = Concatenator;
    SoundCue->LinkGraphNodesFromSoundNodes();

    for (FString name : InputArray) {

        FString path = "SoundWave'/Game/Sounds/All_phrases/" + name + "." + name + "'";
        USoundWave* SoundWave = LoadObjFromPath<USoundWave>(FName(*path));
        if (!SoundWave) {
            UE_LOG(LogTemp, Warning, TEXT("Cant load sound wave: %s"), *path);
            return;
        }
        FDistanceDatum TempDatum;

        if (NodeIndex == 0) {
            TempDatum.FadeInDistanceStart = 0;
            TempDatum.FadeInDistanceEnd = 0;

            TempDatum.FadeOutDistanceStart = SoundWave->GetDuration() + 0.3f;
            TempDatum.FadeOutDistanceEnd = SoundWave->GetDuration() + 0.4f;
        }
        else {
            TempDatum.FadeInDistanceStart = Datum[NodeIndex - 1].FadeOutDistanceStart;
            TempDatum.FadeInDistanceEnd = Datum[NodeIndex - 1].FadeOutDistanceEnd;
            TempDatum.FadeOutDistanceStart = TempDatum.FadeInDistanceStart + SoundWave->GetDuration();
            TempDatum.FadeOutDistanceEnd = TempDatum.FadeInDistanceEnd + SoundWave->GetDuration();
        }

        Datum.Add(TempDatum);

        USoundNodeWavePlayer* WavePlayer = SoundCue->ConstructSoundNode<USoundNodeWavePlayer>();

        WavePlayer->SetSoundWave(SoundWave);
        WavePlayer->GraphNode->NodePosX = -650;
        WavePlayer->GraphNode->NodePosY = -100 * NodeIndex;
        // WavePlayer->bLooping = true;

        SummaryDuration += SoundWave->GetDuration();

        //Crossfade->CreateStartingConnectors();
        Concatenator->CreateStartingConnectors();
        Concatenator->ChildNodes[NodeIndex] = WavePlayer;
        //Crossfade->ChildNodes[NodeIndex] = WavePlayer;
        SoundCue->LinkGraphNodesFromSoundNodes();
        NodeIndex++;
    }
    //Crossfade->CrossFadeInput = Datum;
}

void UPhrasesAudioComponent::PlaySoundWithCrossfade(TArray<FString> InputArray, UBubleTextWidgetClass* Widget) {
    if (InputArray.Num() <= 0)
        return;

   CreateCue(InputArray);
   SetSound(SoundCue);
   
   UWorld* World = GetWorld();
   if (World) {
     //  World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UPhrasesAudioComponent::SetCrossfadeParametr, 0.1f, true);
   } 

   Widget->SeeBotAnswer(InputArray);
   Play();
}

void UPhrasesAudioComponent::SetCrossfadeParametr() {
    if (IsPlaying()) {
        if (TimerCount >= SummaryDuration) {
            Stop();
           // GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);
        }
        else {
            TimerCount += 0.1;
            UE_LOG(LogTemp, Warning, TEXT("Timer count %f"), TimerCount);
            SetFloatParameter("CrossfadeParam", TimerCount);
        }
    }
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

    USoundCue* _SoundCue = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, *FullSoundName.ToString()));
    if (!_SoundCue) {
        UE_LOG(LogTemp, Warning, TEXT("Cant load sound cue: %s"), *FullSoundName.ToString());
        return;
    }

    SetSound(_SoundCue);
    Play();
}