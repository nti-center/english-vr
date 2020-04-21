// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "AIController.h"
#include "Basket.h"
#include "VRPawn.h"
#include "Sound/SoundCue.h"
#include "BubleTextWidgetClass.h"
#include "Components/WidgetComponent.h"
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
    None          UMETA(DisplayName = "None"),
    Taking        UMETA(DisplayName = "Taking"),
    Disappointing UMETA(DisplayName = "Disappointing"),
    Hiding        UMETA(DisplayName = "Hiding"),
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UWidgetComponent* WidgetComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBubleTextWidgetClass* BubbleText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAnimationState AnimationState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> FruitsCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> ToPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> CurrentPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> OutPath;

    UPROPERTY(BlueprintReadWrite)
    int32 WalkingCount;

    UPROPERTY(BlueprintReadWrite)
    AVRPawn* Pawn;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FCanTakeBasketDelegate OnCanTakeBasket;

    UFUNCTION(BlueprintCallable)
    void SetPath(TArray<AActor*> Path);

    UFUNCTION(BlueprintCallable)
    void Go();

    bool TakeBasket(ABasket* NewBasket);

    UFUNCTION(BlueprintCallable)
    void AddFruitRequest(FString Type, int32 Count);

    UFUNCTION(BlueprintCallable)
    void ClearFruitRequests();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override; 

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
