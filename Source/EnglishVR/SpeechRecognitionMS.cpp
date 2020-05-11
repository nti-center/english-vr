#include "SpeechRecognitionMS.h"

using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

USpeechRecognitionMS::USpeechRecognitionMS() {
    PrimaryComponentTick.bCanEverTick = true;
    //700eeb8760144050a16bb579f6c7b545
}

void USpeechRecognitionMS::BeginPlay() {
    Super::BeginPlay();
}

void USpeechRecognitionMS::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpeechRecognitionMS::CreateReognizer(const FString& Subscription, const FString& Region) {
    auto Config = SpeechConfig::FromSubscription(TCHAR_TO_UTF8(*Subscription), TCHAR_TO_UTF8(*Region));
    //Config->SetProperty(PropertyId::SpeechServiceConnection_EndSilenceTimeoutMs, "2000");
    Recognizer = SpeechRecognizer::FromConfig(Config);

    std::function<void(const SpeechRecognitionEventArgs & E)> RecognizedFunction = std::bind(&USpeechRecognitionMS::Recognized, this, std::placeholders::_1);
    Recognizer->Recognized.Connect(RecognizedFunction);

    StartRecognition();
}


void ASpeechRecognitionMS::StopRecognition() {
    Recognizer->StopContinuousRecognitionAsync().get();
}

void ASpeechRecognitionMS::StartRecognition() {
    Recognizer->StartContinuousRecognitionAsync().get();
}

void USpeechRecognitionMS::Recognize(const FString& File) {
    auto Config = SpeechConfig::FromSubscription("700eeb8760144050a16bb579f6c7b545", "westus");
    Config->SetProperty(PropertyId::SpeechServiceConnection_EndSilenceTimeoutMs, "2000");
    auto AudioInput = AudioConfig::FromWavFileInput(TCHAR_TO_UTF8(*(File + ".wav")));
    auto SR = SpeechRecognizer::FromConfig(Config, AudioInput);
    
    auto Result = SR->RecognizeOnceAsync().get();

    ParseResult(Result);
}

void ASpeechRecognitionMS::Recognized(const SpeechRecognitionEventArgs& E) {
    ParseResult(E.Result);
}

void ASpeechRecognitionMS::ParseResult(std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognitionResult> Result) {
    if (Result->Reason == ResultReason::RecognizedSpeech) {
        FString RecognizedString(Result->Text.c_str());
        OnRecognized.Broadcast(RecognizedString);
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

