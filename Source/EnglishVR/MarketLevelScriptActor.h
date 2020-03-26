// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "Basket.h"
#include "MarketLevelScriptActor.generated.h"

UCLASS()
class ENGLISHVR_API AMarketLevelScriptActor : public ALevelScriptActor {
    GENERATED_BODY()

public:
    AMarketLevelScriptActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AMyCharacter* Character;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AMyCharacter> ToCharacterSpawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ABasket* Basket;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ABasket> ToBasketSpawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ATargetPoint* BasketSpawnPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ATargetPoint* CharacterSpawnPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> ToPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AActor*> OutPath;

    UFUNCTION(BlueprintCallable)
    void SpawnCharacter();

    UFUNCTION(BlueprintCallable)
    void SpawnBasket();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
