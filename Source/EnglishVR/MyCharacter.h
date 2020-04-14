// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "AIController.h"
#include "Basket.h"
#include "Sound/SoundCue.h"
#include "PhrasesAudioComponent.h"
#include "UObject/UObjectGlobals.h"
#include "AudioDataTableStruct.h"
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
    bool IsCheck = false;

    UPROPERTY(BlueprintReadWrite)
    bool IsTmp = false;

    UPROPERTY(BlueprintReadWrite)
    bool IsEnd = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> FruitsCount;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, int32> AllRequestsFruitsAndCountList;

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
    void GoAway();

    UFUNCTION(BlueprintCallable)
    void Go();

    UFUNCTION(BlueprintCallable)
    void TakeBasket();
    
    bool TakeBasket(ABasket* NewBasket);

    UFUNCTION(BlueprintCallable)
    bool IsCorrectFruitsCount();

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
