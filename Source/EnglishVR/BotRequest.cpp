
#include "BotRequest.h"


UBotRequest::UBotRequest() {
    PrimaryComponentTick.bCanEverTick = true;

    Http = &FHttpModule::Get();
}

void UBotRequest::BeginPlay()
{
    Super::BeginPlay();
    Request();
}

// Called every frame
void UBotRequest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBotRequest::Request() {

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    ECommand Command = ECommand::Start;
    FString UserID = "Player";
    Request->OnProcessRequestComplete().BindUObject(this, &UBotRequest::ResponseReceived);

    Request->SetURL(TEXT("http://localhost:8989/api/rest/v2.0/ask?query=" + Commands[Command] + "&userId=" + UserID));
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
    Request->SetHeader("Content-Type", TEXT("application/json"));
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("Send request"));
}


void UBotRequest::ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    UE_LOG(LogTemp, Warning, TEXT("Get Answer %s"), *Response->GetContentAsString());

    TSharedPtr<FJsonObject> JsonObject;
    FString ResponseString = Response->GetContentAsString();    
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ResponseString);
    
    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        TSharedPtr<FJsonObject> Response = JsonObject->GetObjectField("response");
        UE_LOG(LogTemp, Warning, TEXT("Bot answer is: %s"), *Response->GetStringField("text"));
        FString TextString = Response->GetStringField("text");
        TextString.RemoveFromEnd(".");
        Reader = TJsonReaderFactory<TCHAR>::Create(TextString);
        if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
            UE_LOG(LogTemp, Warning, TEXT("Actions is: %s"), *JsonObject->GetStringField("Actions"));
            UE_LOG(LogTemp, Warning, TEXT("VoicePhrases is: %s"), *JsonObject->GetStringField("VoicePhrases"));
            
            if (!Actions.Contains(JsonObject->GetStringField("Actions"))) {
                UE_LOG(LogTemp, Warning, TEXT("Undefine action: %s"), *JsonObject->GetStringField("Actions"));
                return;
            }

            OnResponseReceived.Broadcast(Actions[JsonObject->GetStringField("Actions")], ParsePhrasesString(JsonObject->GetStringField("VoicePhrases")));
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Cant deserialize text"));
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response"));
    }
}

TArray<EPhrase> UBotRequest::ParsePhrasesString(const FString& PhrasesString) {
    TArray<EPhrase> PhraseArray;

    TArray<FString> PhraseStringArray;
    PhrasesString.ParseIntoArray(PhraseStringArray, TEXT(" "), true);
    bool IsCorrect = true;
    for (auto& PhraseString : PhraseStringArray) {
        if (!Phrases.Contains(PhraseString)) {
            UE_LOG(LogTemp, Warning, TEXT("Undefine phrase: %s"), *PhraseString);
            IsCorrect = false;
            continue;
        }
        PhraseArray.Add(Phrases[PhraseString]);
    }

    if (!IsCorrect)
        PhraseArray.Empty();

    return PhraseArray;
}



