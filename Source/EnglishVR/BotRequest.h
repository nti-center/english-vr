// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "Engine.h"
#include "BotRequest.generated.h"

UENUM(BlueprintType)
enum class ECommand : uint8 {
    Start UMETA(DisplayName = "Start"),
};

//UENUM(BlueprintType)
//enum class EJsonField : uint8 {
//    Actions      UMETA(DisplayName = "Actions"),
//    VoicePhrases UMETA(DisplayName = "VoicePhrases")
//};

UENUM(BlueprintType)
enum class EAction : uint8 {
    Go UMETA(DisplayName = "Go"),
};

UENUM(BlueprintType)
enum class EPhrase : uint8 {
    Hello UMETA(DisplayName = "Go")
};

const TMap<ECommand, FString> Commands = {
    { ECommand::Start, "cmdStart" }
};

//const TMap<EJsonField, FString> JsonFields = {
//    { EJsonField::Actions,      "Actions" },
//    { EJsonField::VoicePhrases, "VoicePhrases" }
//};

const TMap<FString, EAction> Actions = {
    { "Go", EAction::Go }
};

const TMap<FString, EPhrase> Phrases = {
    { "Hello", EPhrase::Hello }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API UBotRequest : public USceneComponent {
    GENERATED_BODY()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseReceivedDelegate, EAction, Action, TArray<EPhrase>, PhraseArray);

public:
    UBotRequest();
    void Request();
    void ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FResponseReceivedDelegate OnResponseReceived;

private:
    FHttpModule* Http;

    TArray<EPhrase> ParsePhrasesString(const FString& PhrasesString);

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
