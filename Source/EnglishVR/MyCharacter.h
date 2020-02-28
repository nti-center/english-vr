// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "AIController.h"
#include "Sound/SoundCue.h"
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

UCLASS()
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

	UPROPERTY(BlueprintReadWrite)
	UAudioComponent* Audio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* DataTable;

	UPROPERTY(BlueprintReadWrite)
	bool isCheck = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* thisCharacter;

	UPROPERTY(BlueprintReadWrite)
	AAIController* ai;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ToPath;

	UPROPERTY(BlueprintReadWrite)
	TArray<ATargetPoint*> OutPath;

	UPROPERTY(BlueprintReadWrite)
	EStatesEnum EComeState;

	UPROPERTY(BlueprintReadWrite)
	EStatesEnum ENegativeState;

	UPROPERTY(BlueprintReadWrite)
	EStatesEnum EPickupState;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 walkingCount;




	UFUNCTION(BlueprintCallable)
	static void GoToMarket(TArray<AActor*> _toPath, AAIController* _ai, int32 _walkingCount);

	UFUNCTION(BlueprintCallable)
	static void PlayDialog(FName DialogName, UDataTable* _dataTable, UAudioComponent* _audio, bool _isCheck);

	UFUNCTION(BlueprintCallable)
	static bool IsState(EStatesEnum A, EStatesEnum B);

	UFUNCTION(BlueprintCallable)
	static bool IsNotPlaying(UAudioComponent* _audio);





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
