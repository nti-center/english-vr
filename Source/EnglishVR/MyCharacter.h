// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "AudioDataTableStruct.h"
#include "MyCharacter.generated.h"

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

	FAudioDataTableStruct Struct;

	UFUNCTION(BlueprintCallable)
	static void GoToMarket(TArray<AActor*> pathArray, AAIController* _ai);

	UFUNCTION(BlueprintCallable)
	static void PlayDialog(FString DialogName, UDataTable* table, UAudioComponent* _audio, bool check);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
