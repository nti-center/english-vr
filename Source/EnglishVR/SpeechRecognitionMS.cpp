#include "SpeechRecognitionMS.h"

using namespace Microsoft::CognitiveServices::Speech;

USpeechRecognitionMS::USpeechRecognitionMS() {
    PrimaryComponentTick.bCanEverTick = true;

    auto Config = SpeechConfig::FromSubscription("700eeb8760144050a16bb579f6c7b545", "westus");
    Config->SetProperty(PropertyId::SpeechServiceConnection_EndSilenceTimeoutMs, "2000");
    Recognizer = SpeechRecognizer::FromConfig(Config);
}

void USpeechRecognitionMS::BeginPlay() {
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Warning, TEXT("Start recognition"));

    std::function<void(const SpeechRecognitionEventArgs& E)> RecognizedFunction = std::bind(&USpeechRecognitionMS::Recognized, this, std::placeholders::_1);
    Recognizer->Recognized.Connect(RecognizedFunction);
}

void USpeechRecognitionMS::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpeechRecognitionMS::StopRecognition() {
    Recognizer->StopContinuousRecognitionAsync().get();
}

void USpeechRecognitionMS::StartRecognition() {
    Recognizer->StartContinuousRecognitionAsync().get();
}

void USpeechRecognitionMS::Recognized(const SpeechRecognitionEventArgs& E) {
    auto Result = E.Result;
    OnRecognized.Broadcast(FString(Result->Text.c_str()), (int)Result->Reason);

    if (Result->Reason == ResultReason::RecognizedSpeech) {
        FString RecognizedString(Result->Text.c_str());
        UE_LOG(LogTemp, Warning, TEXT("We recognized: %s"), *RecognizedString);
    }
    else if (Result->Reason == ResultReason::NoMatch) {
        UE_LOG(LogTemp, Warning, TEXT("NOMATCH: Speech could not be recognized."));
    }
    else if (Result->Reason == ResultReason::Canceled) {
        auto Cancellation = CancellationDetails::FromResult(Result);
        UE_LOG(LogTemp, Warning, TEXT("CANCELED: Reason = %d"), (int)Cancellation->Reason);

        if (Cancellation->Reason == CancellationReason::Error) {
            UE_LOG(LogTemp, Warning, TEXT("CANCELED: ErrorCode = %d"), (int)Cancellation->ErrorCode);
            FString Details(Cancellation->ErrorDetails.c_str());
            UE_LOG(LogTemp, Warning, TEXT("CANCELED: ErrorDetails ="), *Details);
        }
    }
}

