// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "AIController.h"
#include "Basket.h"
#include "Sound/SoundCue.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "AudioDataTableStruct.h"
#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class EStatesEnum : uint8
{
	NotActive    UMETA(DisplayName = "NotActive"),
	Active      UMETA(DisplayName = "Active"),
	Finished   UMETA(DisplayName = "Finished"),
};

UCLASS(Abstract)
class ENGLISHVR_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	USkeletalMesh* AlternateMeshAsset;


	UPROPERTY(BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* Audio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* DataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* _Table;

	UPROPERTY(BlueprintReadWrite)
	bool isCheck = false;

	UPROPERTY(BlueprintReadWrite)
	bool isTmp = false;

	UPROPERTY(BlueprintReadWrite)
	bool isEnd = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* thisCharacter;

	UPROPERTY(BlueprintReadWrite)
	AAIController* ai;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ToPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> OutPath;

	UPROPERTY(BlueprintReadWrite)
	EStatesEnum EComeState;

	UPROPERTY(BlueprintReadWrite)
	EStatesEnum ENegativeState;

	UPROPERTY(BlueprintReadWrite)
	EStatesEnum EPickupState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 walkingCount;

	UPROPERTY(BlueprintReadWrite)
	ABasket* Basket;


	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FName> DialogList;
	
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, int32> FruitCount;




	UFUNCTION(BlueprintCallable)
	void GoToMarket();

	UFUNCTION(BlueprintCallable)
	void GoAway();

	UFUNCTION(BlueprintCallable)
	void GetABasket();

	UFUNCTION(BlueprintCallable)
	bool IsState(EStatesEnum A, EStatesEnum B);


	UFUNCTION(BlueprintImplementableEvent)
	void PlayDialog(FName DialogName, UDataTable* _dataTable, bool _isCheck);

	UFUNCTION(BlueprintImplementableEvent)
	bool IsNotPlaying();


	UFUNCTION(BlueprintCallable)
	bool IsCorrectFruitsCount(TMap<FString, int32> _A, TMap<FString, int32> _B);

	UFUNCTION(BlueprintCallable)
	void RandomDialogGenerator(TArray<FName> SoundsName);

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
