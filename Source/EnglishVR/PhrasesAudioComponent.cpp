
#include "PhrasesAudioComponent.h"

UPhrasesAudioComponent::UPhrasesAudioComponent() {
    PrimaryComponentTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTable(TEXT("DataTable'/Game/CSV/CrossfadeParametrs.CrossfadeParametrs'"));
    if (_DataTable.Succeeded()) {
        CrossfadeParametrsDataTable = _DataTable.Object;
    }

    //OnAudioFinished.AddDynamic(this, &UPhrasesAudioComponent::PlayNextSound);
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

int UPhrasesAudioComponent::CreateCue(TArray<FString> InputArray) {
    TArray<struct FDistanceDatum> Datum;

    FString SoundCueName = "CueWithCrossfade";
    FString ContextString;
    SoundCue = NewObject<USoundCue>(this, *SoundCueName);

    int32 NodeIndex = 0;
    int32 ErrorIndex = 0;
    PlayersArray.Reset();
    SoundDuration.Reset();

    USoundNodeParamCrossFade* Crossfade = SoundCue->ConstructSoundNode<USoundNodeParamCrossFade>();
    USoundNodeConcatenator* Concatenator = SoundCue->ConstructSoundNode<USoundNodeConcatenator>();

    Crossfade->ParamName = "CrossfadeParam";

    Crossfade->GraphNode->NodePosX = -130;
    Crossfade->GraphNode->NodePosY = 50 * InputArray.Num() / 2;

    SoundCue->FirstNode = Crossfade;
    //SoundCue->FirstNode = Concatenator;
    SoundCue->LinkGraphNodesFromSoundNodes();

    for (FString name : InputArray) {
        FString path = "SoundWave'/Game/Sounds/GirlSounds/" + name + "." + name + "'";
        USoundWave* SoundWave = LoadObjFromPath<USoundWave>(FName(*path));

        if (!SoundWave) {
            ErrorIndex = NodeIndex + 1;
            UE_LOG(LogTemp, Warning, TEXT("Can not find audio file!"));
        }
        else {
            FDistanceDatum TempDatum;

            FCrossfadeDataTableStruct* Row = CrossfadeParametrsDataTable->FindRow<FCrossfadeDataTableStruct>(FName(*name), ContextString, true);
            if (Row) {
                if (NodeIndex == 0) {
                    TempDatum.FadeInDistanceStart = Row->FadeInStart;
                    TempDatum.FadeInDistanceEnd = Row->FadeInEnd;
            
                    TempDatum.FadeOutDistanceStart = Row->FadeOutStart;
                    TempDatum.FadeOutDistanceEnd = Row->FadeOutEnd;
                }
                else {
                    TempDatum.FadeInDistanceStart = SummaryDuration + Row->FadeInStart - (Row->FadeInEnd - Row->FadeInStart );
                    TempDatum.FadeInDistanceEnd = SummaryDuration + Row->FadeInEnd - (Row->FadeInEnd - Row->FadeInStart);
            
                    TempDatum.FadeOutDistanceStart = SummaryDuration + Row->FadeOutStart;
                    TempDatum.FadeOutDistanceEnd = SummaryDuration + Row->FadeOutEnd;
                }
            }
            else {
                if (NodeIndex == 0) {
                    TempDatum.FadeInDistanceStart = 0;
                    TempDatum.FadeInDistanceEnd = 0;

                    TempDatum.FadeOutDistanceStart = SoundWave->GetDuration() - 0.15f;
                    TempDatum.FadeOutDistanceEnd = SoundWave->GetDuration() - 0.05;
                }
                else {
                    TempDatum.FadeInDistanceStart = Datum[NodeIndex - 1].FadeOutDistanceStart;
                    TempDatum.FadeInDistanceEnd = Datum[NodeIndex - 1].FadeOutDistanceEnd;

                    TempDatum.FadeOutDistanceStart = TempDatum.FadeInDistanceStart + SoundWave->GetDuration();
                    TempDatum.FadeOutDistanceEnd = TempDatum.FadeInDistanceEnd + SoundWave->GetDuration();
                }
            }               

            Datum.Add(TempDatum);

            USoundNodeWavePlayer* WavePlayer = SoundCue->ConstructSoundNode<USoundNodeWavePlayer>();

            WavePlayer->SetSoundWave(SoundWave);
            WavePlayer->GraphNode->NodePosX = -650;
            WavePlayer->GraphNode->NodePosY = -100 * NodeIndex;

            WavePlayer->bLooping = true;

            SummaryDuration += SoundWave->GetDuration();

            //Concatenator->CreateStartingConnectors();
            //Concatenator->ChildNodes[NodeIndex] = WavePlayer;

            Crossfade->CreateStartingConnectors();
            Crossfade->ChildNodes[NodeIndex] = WavePlayer;

            SoundCue->LinkGraphNodesFromSoundNodes();

            PlayersArray.Add(WavePlayer);
            SoundDuration.Add(SoundWave->GetDuration());
            NodeIndex++;
        }
    }
    Crossfade->CrossFadeInput = Datum;
    return ErrorIndex;
}

void UPhrasesAudioComponent::PlaySoundWithCrossfade(TArray<FString> InputArray, UWidgetComponent* Widget) {
   if (InputArray.Num() <= 0)
       return;

   int32 Error = CreateCue(InputArray);
   SetSound(SoundCue);

   StepCount = 0;
   CurrentDuration = SoundDuration[StepCount];

   UWorld* World = GetWorld();
   if (World) {
       World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UPhrasesAudioComponent::SetCrossfadeParametr, 0.01f, true);
   }

   WidgetBubble = Cast<UBubleTextWidgetClass>(Widget->GetUserWidgetObject());
   FVector2D WidgetSize = WidgetBubble->SeeBotAnswer(InputArray, Error);
   if(WidgetSize > Widget->GetDrawSize())
       Widget->SetDrawSize(WidgetSize);

   Play();
}

void UPhrasesAudioComponent::SetCrossfadeParametr() {
    if (TimerCount < SummaryDuration) {
        TimerCount += 0.01;

        if (TimerCount >= CurrentDuration - 0.2f) {
            PlayersArray[StepCount]->bLooping = false;
            if (StepCount < PlayersArray.Num() - 1) {
                StepCount++;
                CurrentDuration = CurrentDuration + SoundDuration[StepCount];
            }
        }
        SetFloatParameter("CrossfadeParam", TimerCount);
    }
    else {
        Stop();
        TimerCount = 0;
        SummaryDuration = 0;
        GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);           
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