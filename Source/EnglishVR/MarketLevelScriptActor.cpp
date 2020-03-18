// Fill out your copyright notice in the Description page of Project Settings.


#include "MarketLevelScriptActor.h"

AMarketLevelScriptActor::AMarketLevelScriptActor() {
    PrimaryActorTick.bCanEverTick = true;
    
}

void AMarketLevelScriptActor::BeginPlay() {
    Super::BeginPlay();

}

void AMarketLevelScriptActor::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AMarketLevelScriptActor::SpawnCharacter() {
    if (CharacterSpawnPoint == nullptr || ToCharacterSpawn == nullptr)
        return;

    Character = Cast<AMyCharacter>(GetWorld()->SpawnActor(ToCharacterSpawn, &CharacterSpawnPoint->GetActorTransform()));

    if (Character == nullptr)
        return;

    Character->ToPath = ToPath;
    Character->OutPath = OutPath;
    Character->GoToMarket();
}

void AMarketLevelScriptActor::SpawnBasket() {
    if (BasketSpawnPoint == nullptr || ToBasketSpawn == nullptr)
        return;

    //BasketSpawnPoint->GetActorTransform();

    Basket = Cast<ABasket>(GetWorld()->SpawnActor(ToBasketSpawn, &BasketSpawnPoint->GetActorTransform()));
}

