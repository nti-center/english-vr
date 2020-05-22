// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FruitController.h"
#include "CharacterDataTableStruct.h"
#include "MyCharacter.h"
#include "Basket.h"
#include "BotRequest.h"
#include "FinalTargetPoint.h"
#include "SpeechRecognizerMCSS.h"
#include "SpeechRecognizer.h"
#include "SpeechRecognizerDeepSpeech.h"
#include "MarketLevelScriptActor.generated.h"

UCLASS()
class ENGLISHVR_API AMarketLevelScriptActor : public ALevelScriptActor {
    GENERATED_BODY()

public:
    AMarketLevelScriptActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AVRPawn* VRPawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AMyCharacter* Character;

    UPROPERTY(EditDefaultsOnly)
    UDataTable* CharacterParametrsTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* MarketActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AFruitController* FruitController;

    UPROPERTY(EditAnywhere)
    TArray<FString> RequestedFruits;

    UPROPERTY(EditAnywhere)
    TArray<FString> Boxes;

    UPROPERTY(EditAnywhere)
    bool RandomGeneration = true;

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle FuzeTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AMyCharacter> ToCharacterSpawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ABasket* Basket;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ABasket> ToBasketSpawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ATargetPoint* BasketSpawnPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* CharacterSpawnPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AFinalTargetPoint* MarketPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> ToPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> OutPath;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBotRequest* BotRequest;


    UPROPERTY(EditAnywhere)
    UWidgetComponent* Widget;

    UPROPERTY(BlueprintReadOnly)
    ASpeechRecognizer* SpeechRecognizer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ASpeechRecognizer> SpeechRecognizerType;

    UFUNCTION(BlueprintCallable)
    void SelectSpawnAndDestroyPoint();

    UFUNCTION(BlueprintCallable)
    void SpawnCharacter();

    UFUNCTION(BlueprintCallable)
    void SpawnBasket();

    UFUNCTION()
    bool IsCorrectFruitsCount();

    UFUNCTION()
    void OnBotResponseReceived(EAction Action, TArray<FString> ParamArray, TArray<FString> PhraseArray);

    UFUNCTION()
    void OnTargetPointOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPickupBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPickupBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void OnCharacterCanTakeBasket();

    UFUNCTION()
    void OnBasketFruitCountChanged();

    UFUNCTION()
    void OnCharacterAudioFinished();

    UFUNCTION()
    void OnSpeechRecognized(FString Text);

    UFUNCTION()
    void OnAudioRecorded(FString File);

private:
    void PlayAction(EAction Action, TArray<FString> ParamArray);
    void PlayAudio(TArray<FString> PhraseArray, UWidgetComponent* TextWidget);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
