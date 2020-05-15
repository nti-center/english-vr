// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Fruit.h"
#include "FruitController.generated.h"

UCLASS()
class ENGLISHVR_API AFruitController : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFruitController();

    UPROPERTY(EditDefaultsOnly)
    TArray<FString> RequestedFruits;

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle FuzeTimerHandle;

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
    TMap<FString, int32> AllFruits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fruit")
    TSubclassOf<class AFruit> FruitClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* MarketActor;

    UFUNCTION(BlueprintCallable)
    void SpawnFruits();

    UFUNCTION(BlueprintCallable)
    void CreateFruit(UStaticMeshComponent* Box);

    UFUNCTION(BlueprintCallable)
    void RandomFruitGeneration();

    UFUNCTION(BlueprintCallable)
    void DestroyFallenFruits();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
