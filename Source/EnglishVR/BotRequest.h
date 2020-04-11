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
    Start                       UMETA(DisplayName = "Start"),
    NewCharacterSpawned         UMETA(DisplayName = "NewCharacterSpawned"),
    ReachedMarket               UMETA(DisplayName = "ReachedMarket"),
    BasketOverlapCharacterBegin UMETA(DisplayName = "BasketOverlapCharacterBegin"),
    BasketOverlapCharacterEnd   UMETA(DisplayName = "BasketOverlapCharacterEnd"),
    CorrectFruitsCount          UMETA(DisplayName = "CorrectFruitsCount"),
    IncorrectFruitsCount        UMETA(DisplayName = "IncorrectFruitsCount"),
    CanTakeBasket               UMETA(DisplayName = "CanTakeBasket"),
    BasketTaken                 UMETA(DisplayName = "BasketTaken"),
};

//UENUM(BlueprintType)
//enum class EJsonField : uint8 {
//    Actions      UMETA(DisplayName = "Actions"),
//    VoicePhrases UMETA(DisplayName = "VoicePhrases")
//};

UENUM(BlueprintType)
enum class EAction : uint8 {
    None                   UMETA(DisplayName = "None"),
    Go                     UMETA(DisplayName = "Go"),
    GoToMarket             UMETA(DisplayName = "GoToMarket"),
    SetRequest             UMETA(DisplayName = "SetRequest"),
    TryToTakeBasket        UMETA(DisplayName = "TryToTakeBasket"),
    StopTryingToTakeBasket UMETA(DisplayName = "StopTryingToTakeBasket"),
    StartGrieving          UMETA(DisplayName = "StartGrieving"),
    CheckFruitsCount       UMETA(DisplayName = "CheckFruitsCount"),
    TakeBasket             UMETA(DisplayName = "TakeBasket"),
    GoToHome               UMETA(DisplayName = "GoToHome"),
};

const TMap<ECommand, FString> Commands = {
    { ECommand::Start,                       "cmdStart" },
    { ECommand::NewCharacterSpawned,         "cmdNewCharacterSpawned" },
    { ECommand::ReachedMarket,               "cmdReachedMarket" },
    { ECommand::BasketOverlapCharacterBegin, "cmdBasketOverlapCharacterBegin" },
    { ECommand::BasketOverlapCharacterEnd,   "cmdBasketOverlapCharacterEnd" },
    { ECommand::CorrectFruitsCount,          "cmdCorrectFruitsCount" },
    { ECommand::IncorrectFruitsCount,        "cmdIncorrectFruitsCount" },
    { ECommand::CanTakeBasket,               "cmdCanTakeBasket" },
    { ECommand::BasketTaken,                 "cmdBasketTaken" },
};

//const TMap<EJsonField, FString> JsonFields = {
//    { EJsonField::Actions,      "Actions" },
//    { EJsonField::VoicePhrases, "VoicePhrases" }
//};

const TMap<FString, EAction> Actions = {
    { "Go",                     EAction::Go },
    { "GoToMarket",             EAction::GoToMarket },
    { "SetRequest",             EAction::SetRequest },
    { "TryToTakeBasket",        EAction::TryToTakeBasket },
    { "StartGrieving",          EAction::StartGrieving },
    { "CheckFruitsCount",       EAction::CheckFruitsCount },
    { "StopTryingToTakeBasket", EAction::StopTryingToTakeBasket },
    { "TakeBasket",             EAction::TakeBasket },
    { "GoToHome",               EAction::GoToHome },
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
