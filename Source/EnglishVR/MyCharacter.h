// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "AIController.h"
#include "Basket.h"
#include "Sound/SoundCue.h"
#include "PhrasesAudioComponent.h"
#include "EdGraph/EdGraph.h"
#include "Sound/SoundNodeWavePlayer.h"
#include "Sound/SoundNodeParamCrossFade.h"
#include "Sound/SoundNodeConcatenator.h"
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

UENUM(BlueprintType)
enum class EAnimationState : uint8 {
    None     UMETA(DisplayName = "None"),
    Taking   UMETA(DisplayName = "Taking"),
    Grieving UMETA(DisplayName = "Grieving"),
};

UCLASS(Abstract)
class ENGLISHVR_API AMyCharacter : public ACharacter {
    GENERATED_BODY()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCanTakeBasketDelegate);

public:
    // Sets default values for this character's properties
    AMyCharacter();

    UPROPERTY(BlueprintReadWrite)
    UBoxComponent* Box;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Audio Conmponent", meta = (AllowPrivateAccess = "true"))
    UAudioComponent* Audio;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UPhrasesAudioComponent* PhrasesAudio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAnimationState AnimationState;

    UPROPERTY(BlueprintReadWrite)
    USoundCue* SoundCue;

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle FuzeTimerHandle;

    UPROPERTY(BlueprintReadWrite)
    float TimerCount = 0;

    UPROPERTY(BlueprintReadWrite)
    float SummaryDuration = 0;

#pragma region DataTable
    //������� ��� ��������� ����� ����� �������
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

    //������� ��� ��������� ���������� �������
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
    TArray<AActor*> CurrentPath;

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

    //��� ������������ �����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> PathArray;

    UPROPERTY(BlueprintReadWrite)
    int32 PlayingSoundNumber = 0;

#pragma region VariablesForRandomRequestGeneration

    //���������� ��� ��������� ���������� �������� NPC
    UPROPERTY(BlueprintReadWrite)
    int32 RequestCount;

    //���������� ��� ��������, ����� ������ ���� ������
    UPROPERTY(BlueprintReadWrite)
    int32 Counter = 1;

    //���������� ��� ������� ����� ������� ���� ��������, ������������ ��� ����
    //��� �� ������ ����� �� �����������
    UPROPERTY(BlueprintReadWrite)
    int32 RequestPhrasesArrayLength = 0;

    //��� ������ �������������� �� ���������� ����
    UPROPERTY(BlueprintReadWrite)
    FString PreviousFruit;

    //������ ��� � ���������� ������� ��������� �� ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> FruitsCount;

    //������ ����� � ������ ������� �������
    UPROPERTY(BlueprintReadWrite)
    TArray <FName> RequestFullPhrasesArray;

    //������ �������� ���� ��������, ���������� �� ������� request
    UPROPERTY(BlueprintReadWrite)
    TArray<FName> RequestPhrasesArray;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, int32> AllRequestsFruitsAndCountList;

#pragma  endregion

    UPROPERTY(BlueprintReadWrite)
    ABasket* Basket;

    UPROPERTY(BlueprintReadWrite)
    TMap<FName, FName> DialogList;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FCanTakeBasketDelegate OnCanTakeBasket;

    UFUNCTION(BlueprintCallable)
    void SetPath(TArray<AActor*> Path);

    UFUNCTION(BlueprintCallable)
    void GoToMarket();

    UFUNCTION(BlueprintCallable)
    void CreateCue(TArray<FString>InputArray);

    UFUNCTION(BlueprintCallable)
    void PlaySoundWithCrossfade(FString SoundNameString);

    UFUNCTION(BlueprintCallable)
    void SetCrossfadeParametr();

    UFUNCTION(BlueprintCallable)
    void GoAway();

    UFUNCTION(BlueprintCallable)
    void Go();

    UFUNCTION(BlueprintCallable)
    void TakeBasket();
    
    bool TakeBasket(ABasket* NewBasket);

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

    UFUNCTION(BlueprintCallable)
    void AddFruitRequest(FString Type, int32 Count);

    UFUNCTION(BlueprintCallable)
    void ClearFruitRequests();

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override; 

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
