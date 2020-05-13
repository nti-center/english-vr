// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "Basket.h"
#include "Fruit.h"
#include "BotRequest.h"
#include "FinalTargetPoint.h"
#include "SpeechRecognitionMS.h"
#include "MarketLevelScriptActor.generated.h"

UCLASS()
class ENGLISHVR_API AMarketLevelScriptActor : public ALevelScriptActor {
    GENERATED_BODY()

public:
    AMarketLevelScriptActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AMyCharacter* Character;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* MarketActor;

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

    UPROPERTY(EditDefaultsOnly)
    UDataTable* DataTable;

    UPROPERTY(EditDefaultsOnly)
    UStaticMesh* FruitMesh;

    UPROPERTY(EditDefaultsOnly)
    TArray<FString> FruitType;

    UPROPERTY(EditDefaultsOnly)
    TArray <FString> FruitPath;

    UPROPERTY(EditDefaultsOnly)
    FString RandomFruitPath;

    UPROPERTY(EditDefaultsOnly)
    FString RandomFruitType;

    UPROPERTY(EditDefaultsOnly)
    int32 counter;

    UPROPERTY(EditDefaultsOnly)
    TMap<FString, int32> AllFruits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fruit")
    TSubclassOf<class AFruit> FruitClass;

    UPROPERTY(EditAnywhere)
    UWidgetComponent* Widget;

    UPROPERTY(EditDefaultsOnly)
    USpeechRecognitionMS* SpeechRecognition;

    UFUNCTION(BlueprintCallable)
    void SelectSpawnAndDestroyPoint();

    UFUNCTION(BlueprintCallable)
    void SpawnCharacter();

    UFUNCTION(BlueprintCallable)
    void SpawnBasket();

    UFUNCTION(BlueprintCallable)
    void SpawnFruits();

    UFUNCTION(BlueprintCallable)
    void RandomFruitGeneration();

    UFUNCTION(BlueprintCallable)
    void DestroyFruits();

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
    void OnSpeechRecognized(FString Text, int Reason);

private:
    void PlayAction(EAction Action, TArray<FString> ParamArray);
    void PlayAudio(TArray<FString> PhraseArray, UWidgetComponent* TextWidget);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
