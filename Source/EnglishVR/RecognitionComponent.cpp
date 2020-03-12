// Fill out your copyright notice in the Description page of Project Settings.

#include "RecognitionComponent.h"

// Sets default values for this component's properties
URecognitionComponent::URecognitionComponent() {
    PrimaryComponentTick.bCanEverTick = true;

    Http = &FHttpModule::Get();
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
    FString IamToken = "CggVAgAAABoBMxKABEvgQBNWQ9qj1tM2T00ML12Ul5Le92fp5Hd6x3FBn1VyKCKIMQ9e-zmkSJG5Md3bW1eg0w_UM4q0acsRveY_zpWsC4RePzvPslFJaoT_BqyQ5_whFWPz3ZYNrB1G9IAyD9B2xioZchfMpPyEEtNy9BsI0dA_f39rAoFKxxd3H1j5b87Lwd0zXcT34-8sBG6uGZpkSQVjjifobwnKHuM9uWy8EzN7epMoNvlfk0PQdmc2vVo-W0Kjc9nd32aOE9GpY-ZY65SAUzDDRCBl-8kuhpjU0fUFoUEJfNxacgPl8_lo4J2Ra7qqxtWFDDnGKn5DuKkwhXjj2QjbiHfA1Fd7K2nlCqwat2Q8gV7Xn749zau22JlQmyJ1iR-izCZn3TBoVdBK0Yre3QY5LSvTp_Ih3FNOP043Nqs4e6OcWWEzj7j5YTf8_3_FajEnBMt36dWpV2wG5B5swgnGgCs-7a_WRGJxgiremFgMEm_o6HXpS87bLoHJLmvzH9MRWZbSDcP1VQnR1Jw-adptvk3sR6bCpZn3aVnz_Fi3XZ7eoQXogLi7zU8gREiG3JZMWLGGps_9SCfxrHcKm31vvy_s9SzKKH7HLXFJ9DMZSeY4ssMk0UW5aQ7klFmo-EGiukRj6tW9xi7Zs1ktvGMckgMpsewZvNbfoLOgG785h4wPqZtBBhuCGiQQmLym8wUY2I2p8wUiFgoUYWplZjJxcWl1dm0wbmVmMWx0aDY=";

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &URecognitionComponent::OnResponseReceived);
  
    Request->SetURL(TEXT("https://stt.api.cloud.yandex.net/speech/v1/stt:recognize?topic=general&lang=en-US&folderId=") + FolderId);
    Request->SetVerb("POST");
    Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + IamToken);
    Request->SetContent(Data);
    Request->ProcessRequest();

    UE_LOG(LogTemp, Warning, TEXT("Send request"));
}

void URecognitionComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString());
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response"));
    }
}

