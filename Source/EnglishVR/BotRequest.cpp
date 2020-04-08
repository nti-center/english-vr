
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

    FString Question = "Hello,+who+are+you?";
    FString UserID = "1234567890";
    Request->OnProcessRequestComplete().BindUObject(this, &UBotRequest::ResponseReceived);

    Request->SetURL(TEXT("http://localhost:8989/api/rest/v1.0/ask?question=" + Question + "&userid=" + UserID));
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
    Request->SetHeader("Content-Type", TEXT("application/json"));
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("Send request"));
}


void UBotRequest::ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    TSharedPtr<FJsonObject> JsonObject;

    UE_LOG(LogTemp, Warning, TEXT("Get Answer %s"), *Response->GetContentAsString());

    FString ResponseString = Response->GetContentAsString();
    int32 FirstPosition = ResponseString.Find("{");
    int32 EndPosition = ResponseString.Find("}", ESearchCase::CaseSensitive, ESearchDir::FromEnd);
    ResponseString = ResponseString.Mid(FirstPosition, EndPosition);

    UE_LOG(LogTemp, Warning, TEXT("Format string %s"), *ResponseString);

    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ResponseString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        TSharedPtr<FJsonObject> obj = JsonObject->GetObjectField("response");
        UE_LOG(LogTemp, Warning, TEXT("Bot answer is: %s"), *obj->GetStringField("answer"));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response")); 
    }
}



