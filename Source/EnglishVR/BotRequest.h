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
    AudioFinished               UMETA(DisplayName = "AudioFinished"),
    BasketOverlapCharacterBegin UMETA(DisplayName = "BasketOverlapCharacterBegin"),
    BasketOverlapCharacterEnd   UMETA(DisplayName = "BasketOverlapCharacterEnd"),
    BasketTaken                 UMETA(DisplayName = "BasketTaken"),
    CanTakeBasket               UMETA(DisplayName = "CanTakeBasket"),
    CorrectFruitsCount          UMETA(DisplayName = "CorrectFruitsCount"),
    IncorrectFruitsCount        UMETA(DisplayName = "IncorrectFruitsCount"),
    NewCharacterSpawned         UMETA(DisplayName = "NewCharacterSpawned"),
    ReachedMarket               UMETA(DisplayName = "ReachedMarket"),
    Start                       UMETA(DisplayName = "Start"),
};

//UENUM(BlueprintType)
//enum class EJsonField : uint8 {
//    Actions      UMETA(DisplayName = "Actions"),
//    VoicePhrases UMETA(DisplayName = "VoicePhrases")
//};

UENUM(BlueprintType)
enum class EAction : uint8 {
    AddRequest             UMETA(DisplayName = "AddRequest"),
    CheckFruitsCount       UMETA(DisplayName = "CheckFruitsCount"),
    Go                     UMETA(DisplayName = "Go"),
    GoToHome               UMETA(DisplayName = "GoToHome"),
    GoToMarket             UMETA(DisplayName = "GoToMarket"),
    None                   UMETA(DisplayName = "None"),
    SetRequest             UMETA(DisplayName = "SetRequest"),
    StartDisappointing     UMETA(DisplayName = "StartDisappointing"),
    StopTryingToTakeBasket UMETA(DisplayName = "StopTryingToTakeBasket"),
    TakeBasket             UMETA(DisplayName = "TakeBasket"),
    TryToTakeBasket        UMETA(DisplayName = "TryToTakeBasket"),
};

const TMap<ECommand, FString> Commands = {
    { ECommand::AudioFinished,               "cmdAudioFinished" },
    { ECommand::BasketOverlapCharacterBegin, "cmdBasketOverlapCharacterBegin" },
    { ECommand::BasketOverlapCharacterEnd,   "cmdBasketOverlapCharacterEnd" },
    { ECommand::BasketTaken,                 "cmdBasketTaken" },
    { ECommand::CanTakeBasket,               "cmdCanTakeBasket" },
    { ECommand::CorrectFruitsCount,          "cmdCorrectFruitsCount" },
    { ECommand::IncorrectFruitsCount,        "cmdIncorrectFruitsCount" },
    { ECommand::NewCharacterSpawned,         "cmdNewCharacterSpawned" },
    { ECommand::ReachedMarket,               "cmdReachedMarket" },
    { ECommand::Start,                       "cmdStart" },
};

//const TMap<EJsonField, FString> JsonFields = {
//    { EJsonField::Actions,      "Actions" },
//    { EJsonField::VoicePhrases, "VoicePhrases" }
//};

const TMap<FString, EAction> Actions = {
    { "AddRequest",             EAction::AddRequest },
    { "CheckFruitsCount",       EAction::CheckFruitsCount },
    { "GoToHome",               EAction::GoToHome },
    { "GoToMarket",             EAction::GoToMarket },
    { "SetRequest",             EAction::SetRequest },
    { "StartDisappointing",     EAction::StartDisappointing },
    { "StopTryingToTakeBasket", EAction::StopTryingToTakeBasket },
    { "TakeBasket",             EAction::TakeBasket },
    { "TryToTakeBasket",        EAction::TryToTakeBasket },
    { "Go",                     EAction::Go },
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API UBotRequest : public USceneComponent {
    GENERATED_BODY()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FResponseReceivedDelegate, EAction, Action, TArray<FString>, ParamArray, TArray<FString>, PhraseArray);

public:
    UBotRequest();
    void Request(ECommand Command);
    void ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FResponseReceivedDelegate OnResponseReceived;

private:
    FHttpModule* Http;

    TArray<FString> ParsePhrasesString(const FString& PhrasesString);

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
