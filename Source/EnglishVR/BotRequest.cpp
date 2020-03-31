
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
    //Request->SetVerb("POST");
    Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
    Request->SetHeader("Content-Type", TEXT("application/json"));
    //Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("Send request"));
}


void UBotRequest::ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid()) {
        FString string = JsonObject->GetStringField("answer");
        UE_LOG(LogTemp, Warning, TEXT("%s %s"), *Response->GetContentAsString(), *string); 
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response")); 
    }
}



