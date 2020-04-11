
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

    TArray<TSharedPtr<FJsonValue>> JsonArray;

    if (!bWasSuccessful || !Response.IsValid())
        return;

    UE_LOG(LogTemp, Warning, TEXT("Get Answer %s"), *Response->GetContentAsString());

    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, JsonArray)) {
        TSharedPtr<FJsonObject> obj = JsonArray[0]->AsObject()->GetObjectField("response");
        UE_LOG(LogTemp, Warning, TEXT("Question is: %s Bot answer is: %s"), *obj->GetStringField("question"), *obj->GetStringField("answer"));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response")); 
    }
}



