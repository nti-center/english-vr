// Fill out your copyright notice in the Description page of Project Settings.

#include "RecognitionComponent.h"

// Sets default values for this component's properties
URecognitionComponent::URecognitionComponent() {
    PrimaryComponentTick.bCanEverTick = true;

    Http = &FHttpModule::Get();

    //OnResponseReceived.AddDynamic(this, &URecognitionComponent::ResponseReceived);
}

// Called when the game starts
void URecognitionComponent::BeginPlay() {        
    Super::BeginPlay();

    //Recognize("C:/Projects/TestYandexApi/TMP1");
}

// Called every frame
void URecognitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URecognitionComponent::Recognize(const FString &FilePath) {
    system((std::string("ffmpeg -i ") + std::string(TCHAR_TO_UTF8(*FilePath)) + std::string(".wav ") +
        std::string(TCHAR_TO_UTF8(*FilePath)) + std::string(".opus")).c_str());

    TArray<uint8> Data;
    bool IsLoad = FFileHelper::LoadFileToArray(Data, *(FilePath + ".opus"));
    system((std::string("rm ") + std::string(TCHAR_TO_UTF8(*FilePath)) + std::string(".opus")).c_str());

    if (!IsLoad) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find file"));
        return;
    }

    FString FolderId = "b1g0fapccdqlq2a46079";
    FString IamToken = "CggVAgAAABoBMxKABBSg9HYxeDvfGjG6nY9pKUA9UA6vytnCVGub3_S7zGcpa1ypRusYQxHnMHaduXEZ5Vidqpl_TfTGxZWxu8gSAVGtfq7GrMN0shxrUsYLwURkc0gyvT4i8gwEbm6Qh8XEcDFEoQycK1GU-2GbrCIRuNte_1y6tpQWxcFDqp7qiP7kwjLygE4p4dN3LZ1bPZZBBG7Zx4ThFWPHRc2QET21wAfmQeOsOd50t5zxpvK55q57GY_I9OK5amyXvVKunskdMo1vL5Hmb5qvgvEPGfQWoIQb2icc-zX1JIhdn7s77PoxDojcOuLIVDNVDPS92HT7wzoKe8qeRDG-At4gqz39hDlaeVoyhFWqn5HWgojsZ3HbEaqg2eHHeOhp8IjeMujzSqaL-FCeXtLsLlf70Q_S-n6h3LHvo53AAW_Q8PbgKTzaxRg18Mf1whJq-OJEC9v6ERjdGbTfxJr5p4C6p7QAhuXIzYmVXZsjxlLXejwBc_30nX2_luYDmAnX0-JAMv5cLaT8IKBnQouKFBi-V0BFw81MAGF4Bo3pPyO4MdASNU6OB8GwezjNDx8scL0pJxKsYpzIEeUJ-rxmFCHibXA0tdtCARWdZnteCP5UX0WbbfA0aW98Wzg3qUF9RDdWHC-NICCKNLTdpji5MtkOOJEp53TThslZNO0QC7INPwbUxCpnGiQQ6eKr8wUYqbSu8wUiFgoUYWplZjJxcWl1dm0wbmVmMWx0aDY=";

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &URecognitionComponent::ResponseReceived);
  
    Request->SetURL(TEXT("https://stt.api.cloud.yandex.net/speech/v1/stt:recognize?topic=general&lang=en-US&folderId=") + FolderId);
    Request->SetVerb("POST");
    Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + IamToken);
    Request->SetContent(Data);
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("Send request"));
}

void URecognitionComponent::ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {    
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid()) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString());
        OnResponseReceived.Broadcast(JsonObject->GetStringField(TEXT("result")));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response"));
        OnResponseReceived.Broadcast(TEXT("Error"));
    }

    
}

