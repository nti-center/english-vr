#include "SpeechRecognitionMS.h"

using namespace Microsoft::CognitiveServices::Speech;

USpeechRecognitionMS::USpeechRecognitionMS() {
    PrimaryComponentTick.bCanEverTick = true;

    auto config = SpeechConfig::FromSubscription("700eeb8760144050a16bb579f6c7b545", "westus");
    Recognizer = SpeechRecognizer::FromConfig(config);
}

void USpeechRecognitionMS::BeginPlay() {
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Warning, TEXT("Start recognition"));

    Recognizer->Recognized.Connect([](const SpeechRecognitionEventArgs& e) {
            UE_LOG(LogTemp, Warning, TEXT("EVENT"));
            auto Result = e.Result;
            if (Result->Reason == ResultReason::RecognizedSpeech) {
                FString Recognized(Result->Text.c_str());
                UE_LOG(LogTemp, Warning, TEXT("We recognized: %s"), *Recognized);
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
            
        });

    Recognizer->StartContinuousRecognitionAsync().get();
}

void USpeechRecognitionMS::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpeechRecognitionMS::StopRecognition() {
    Recognizer->StopContinuousRecognitionAsync().get();
}

