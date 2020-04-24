// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Fruit.h"
#include "Engine/DataTable.h"
#include "AudioDataTableStruct.h"
#include "FruitBoxController.generated.h"



UCLASS()
class ENGLISHVR_API AFruitBoxController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFruitBoxController();

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle FuzeTimerHandle;

    UPROPERTY(EditDefaultsOnly)
    UDataTable* DataTable;

    UPROPERTY(EditDefaultsOnly)
    TArray <FString> FruitType;

    UPROPERTY(EditDefaultsOnly)
    int32 counter;

    UPROPERTY(EditDefaultsOnly)
    TMap<FString, int32> AllFruits;

    UPROPERTY(EditDefaultsOnly)
    TArray <FString> FruitPath;

    UFUNCTION(BlueprintCallable)
    TArray<FString> RandomFruitGeneration();

    UFUNCTION(BlueprintCallable)
    void DestroyFruits();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
