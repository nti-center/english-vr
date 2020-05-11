#include "SpeechRecognitionMS.h"

using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

ASpeechRecognitionMS::ASpeechRecognitionMS() {
    PrimaryActorTick.bCanEverTick = true;

    FString ConfigString;
    bool IsLoad = FFileHelper::LoadFileToString(ConfigString, *(FPaths::ProjectDir() + "/SpeechRecognition/MicrosoftCognitiveServicesSpeech/config.txt"));

    if (!IsLoad) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find MicrosoftCognitiveServicesSpeech/config.txt"));
        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ConfigString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        Subscription = JsonObject->GetStringField("Subscription");
        Region = JsonObject->GetStringField("Region");
        EndSilenceTimeoutMs = JsonObject->GetStringField("EndSilenceTimeoutMs");
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize config"));
    }

    auto Config = SpeechConfig::FromSubscription(TCHAR_TO_UTF8(*Subscription), TCHAR_TO_UTF8(*Region));
    Config->SetProperty(PropertyId::SpeechServiceConnection_EndSilenceTimeoutMs, TCHAR_TO_UTF8(*EndSilenceTimeoutMs));
    Recognizer = SpeechRecognizer::FromConfig(Config);

    std::function<void(const SpeechRecognitionEventArgs & E)> RecognizedFunction = std::bind(&ASpeechRecognitionMS::Recognized, this, std::placeholders::_1);
    Recognizer->Recognized.Connect(RecognizedFunction);
}

void ASpeechRecognitionMS::BeginPlay() {
    Super::BeginPlay();

    StartRecognition();
}

void ASpeechRecognitionMS::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void ASpeechRecognitionMS::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    StopRecognition();

    Super::EndPlay(EndPlayReason);
}

void ASpeechRecognitionMS::StopRecognition() {
    Recognizer->StopContinuousRecognitionAsync().get();
}

void ASpeechRecognitionMS::StartRecognition() {
    Recognizer->StartContinuousRecognitionAsync().get();
}

void ASpeechRecognitionMS::Recognize(const FString& File) {
    auto Config = SpeechConfig::FromSubscription(TCHAR_TO_UTF8(*Subscription), TCHAR_TO_UTF8(*Region));
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

