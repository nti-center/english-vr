// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeechRecognizerDeepSpeech.h"
#include <string>

ASpeechRecognizerDeepSpeech::ASpeechRecognizerDeepSpeech() {
    PrimaryActorTick.bCanEverTick = true;

    Http = &FHttpModule::Get();
    InitSocket();
}

void ASpeechRecognizerDeepSpeech::BeginPlay() {
    Super::BeginPlay();

    Socket->Connect(*Address);
    StartRecognition();

    GetWorld()->GetTimerManager().SetTimer(RecieveTimer, this, &ASpeechRecognizerDeepSpeech::RecieveData, 0.5f, true);
}

void ASpeechRecognizerDeepSpeech::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void ASpeechRecognizerDeepSpeech::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    GetWorld()->GetTimerManager().ClearTimer(RecieveTimer);

    Super::EndPlay(EndPlayReason);
}

void ASpeechRecognizerDeepSpeech::InitSocket() {
    Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_SOCKET"), false);
    Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

    FIPv4Address ServerIP;
    FIPv4Address::Parse(TEXT("127.0.0.1"), ServerIP);

    Address->SetIp(ServerIP.Value);
    Address->SetPort(5001);
}

void ASpeechRecognizerDeepSpeech::RecieveData() {
    TArray<uint8> ReceivedData;
    uint32 Size;

    while (Socket->HasPendingData(Size)) {
        ReceivedData.Init(0, 100000);
        int32 SizeRead = 0;
        Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), SizeRead);
    }

    if (ReceivedData.Num() > 0) {
        std::string DataString(reinterpret_cast<const char*>(ReceivedData.GetData()), ReceivedData.Num());
        OnRecognized.Broadcast(FString(DataString.c_str()));
    }
}

void ASpeechRecognizerDeepSpeech::Recognize(const FString& File) {
    TArray<uint8> Data;
    bool IsLoad = FFileHelper::LoadFileToArray(Data, *(File + ".wav"));

    if (!IsLoad) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find file"));
        return;
    }

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ASpeechRecognizerDeepSpeech::ResponseReceived);

    Request->SetURL(TEXT("http://127.0.0.1:5000/recognize"));
    Request->SetVerb("POST");
    Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));
    Request->SetContent(Data);
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("Send request"));
}

void ASpeechRecognizerDeepSpeech::ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
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
