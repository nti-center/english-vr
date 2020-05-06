// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeechRecognizerDeepSpeech.h"

USpeechRecognizerDeepSpeech::USpeechRecognizerDeepSpeech() {
    PrimaryComponentTick.bCanEverTick = true;

    Http = &FHttpModule::Get();
}

void USpeechRecognizerDeepSpeech::BeginPlay() {
    Super::BeginPlay();
}

void USpeechRecognizerDeepSpeech::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpeechRecognizerDeepSpeech::Recognize(const FString& File) {
    TArray<uint8> Data;
    bool IsLoad = FFileHelper::LoadFileToArray(Data, *(File + ".wav"));

    if (!IsLoad) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find file"));
        return;
    }

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &USpeechRecognizerDeepSpeech::ResponseReceived);

    Request->SetURL(TEXT("http://127.0.0.1:5000/recognize"));
    Request->SetVerb("POST");
    Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));
    Request->SetContent(Data);
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("Send request"));
}

void USpeechRecognizerDeepSpeech::ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid()) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString());
        OnRecognized.Broadcast(JsonObject->GetStringField(TEXT("text")));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response"));
    }
}
