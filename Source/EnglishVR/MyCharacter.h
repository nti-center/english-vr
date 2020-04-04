// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "AIController.h"
#include "Basket.h"
#include "Sound/SoundCue.h"
#include "UObject/UObjectGlobals.h"
#include "AudioDataTableStruct.h"
#include "FruitSoundDataTableStruct.h"
#include "NumbersSoundDataTableStruct.h"
#include "SoundDataTableStruct.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class EStatesEnum : uint8 {
    NotActive UMETA(DisplayName = "NotActive"),
    Active    UMETA(DisplayName = "Active"),
	Process   UMETA(DisplayName = "Process"),
    Whaiting   UMETA(DisplayName = "Whaiting"),
    Finished  UMETA(DisplayName = "Finished"),
};

UCLASS(Abstract)
class ENGLISHVR_API AMyCharacter : public ACharacter {
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AMyCharacter();

    UPROPERTY(BlueprintReadWrite)
    UBoxComponent* Box;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Audio Conmponent", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* Audio;

#pragma region DataTable
    //Таблицы для генерации общей части диалога
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* GreetingsTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* GratitudeTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* PaymentTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* GoodbyeTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* ErrorsTable;

    //Таблицы для генерации составного запроса
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* RequestTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* NumberTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* FruitTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* FruitsTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* EndingTable;

    UPROPERTY(EditDefaultsOnly)
    UDataTable* TmpTable;

    UPROPERTY(EditDefaultsOnly)
    UDataTable* DataTable;
#pragma endregion

    UPROPERTY(BlueprintReadWrite)
    bool IsCheck = false;

    UPROPERTY(BlueprintReadWrite)
    bool IsTmp = false;

    UPROPERTY(BlueprintReadWrite)
    bool IsEnd = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> ToPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> OutPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStatesEnum EComeState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStatesEnum ENegativeState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStatesEnum EPickupState;

    UPROPERTY(BlueprintReadWrite)
    int32 WalkingCount;

    //Для проигрывания звука
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> PathArray;

    UPROPERTY(BlueprintReadWrite)
    int32 PlayingSoundNumber = 0;

#pragma region VariablesForRandomRequestGeneration

    //Переменная для генерации количества запросов NPC
    UPROPERTY(BlueprintReadWrite)
    int32 RequestCount;

    //Переменная для подсчета, какой сейчас идет запрос
    UPROPERTY(BlueprintReadWrite)
    int32 Counter = 1;

    //Переменная для задания длины массива фраз запросов, используется для того
    //что бы данные фразы не повторялись
    UPROPERTY(BlueprintReadWrite)
    int32 RequestPhrasesArrayLength = 0;

    //Тип фрукта запрашиваемого на предыдущем шаге
    UPROPERTY(BlueprintReadWrite)
    FString PreviousFruit;

    //Хранит тип и количество фруктов ожидаемых на данном шаге
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> FruitsCount;

    //Массив путей к фразам полного запроса
    UPROPERTY(BlueprintReadWrite)
    TArray <FName> RequestFullPhrasesArray;

    //Массив названий всех запросов, полученных из таблицы request
    UPROPERTY(BlueprintReadWrite)
    TArray<FName> RequestPhrasesArray;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, int32> AllRequestsFruitsAndCountList;

#pragma  endregion

    UPROPERTY(BlueprintReadWrite)
    ABasket* Basket;

    UPROPERTY(BlueprintReadWrite)
    TMap<FName, FName> DialogList;

    UFUNCTION(BlueprintCallable)
    void GoToMarket();

    UFUNCTION(BlueprintCallable)
    void PlaySoundFromAIML(FString SoundNameString);

    UFUNCTION(BlueprintCallable)
    void PlaySound();

    UFUNCTION(BlueprintCallable)
    void GoAway();

    UFUNCTION(BlueprintCallable)
    void TakeBasket();

    UFUNCTION(BlueprintImplementableEvent)
    void PlayDialog(FName DialogName, bool _isCheck);

    UFUNCTION(BlueprintImplementableEvent)
    bool IsNotPlaying();

    UFUNCTION(BlueprintImplementableEvent)
    void PlayRequestList(const TArray<FName>& RequestList, int32 ListLength,  bool check);

    UFUNCTION(BlueprintCallable)
    bool IsCorrectFruitsCount();

    UFUNCTION(BlueprintCallable)
    void RandomDialogGenerator(TArray<FName> SoundsName);

    UFUNCTION(BlueprintCallable)
    void RandomRequestGenerator();

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override; 
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReasonType) override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
