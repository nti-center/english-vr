
#include "BotRequest.h"


UBotRequest::UBotRequest() {
    PrimaryComponentTick.bCanEverTick = true;

    Http = &FHttpModule::Get();
}

void UBotRequest::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UBotRequest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBotRequest::Request(ECommand Command) {
    Request(Commands[Command]);
}

void UBotRequest::Request(ECommand Command, TArray<FString> Params) {
    FString RequestString = Commands[Command];
    for (auto& Param: Params)
        RequestString += " " + Param;
    Request(RequestString);
}

void UBotRequest::Request(FString RequestString) {
    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    FString UserID = "Player";
    Request->OnProcessRequestComplete().BindUObject(this, &UBotRequest::ResponseReceived);

    Request->SetURL(TEXT("http://localhost:8989/api/rest/v2.0/ask?query=" + RequestString + "&userId=" + UserID));
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
    Request->SetHeader("Content-Type", TEXT("application/json"));
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("Send request"));
}


void UBotRequest::ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    UE_LOG(LogTemp, Warning, TEXT("Get Answer %s"), *Response->GetContentAsString());

    TSharedPtr<FJsonObject> JsonObject;
    FString ResponseString = Response->GetContentAsString().Replace(TEXT("\\n"), TEXT(" "));
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
            
            EAction Action = Action = EAction::None;             
            if (Actions.Contains(JsonObject->GetStringField("Actions"))) {
                Action = Actions[JsonObject->GetStringField("Actions")];
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Undefine action: %s"), *JsonObject->GetStringField("Actions"));
            }

            TArray<FString> Params;
            for (auto& Value : JsonObject->GetArrayField("Params")) {
                Params.Add(Value.Get()->AsString());
            }
            OnResponseReceived.Broadcast(Action, Params, ParsePhrasesString(JsonObject->GetStringField("VoicePhrases")));
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Cant deserialize text"));
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response"));
    }
}

TArray<FString> UBotRequest::ParsePhrasesString(const FString& PhrasesString) {
    TArray<FString> PhraseArray;
    PhrasesString.ParseIntoArray(PhraseArray, TEXT(" "), true);
    return PhraseArray;
}



