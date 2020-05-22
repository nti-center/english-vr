
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
        RequestString += "+" + Param;
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
    FString ResponseString = Response->GetContentAsString().Replace(TEXT("\\n"), TEXT(" "));
    OnResponseReceived.Broadcast(ResponseString);
}



