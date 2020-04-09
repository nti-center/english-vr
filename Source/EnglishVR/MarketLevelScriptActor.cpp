// Fill out your copyright notice in the Description page of Project Settings.


#include "MarketLevelScriptActor.h"

AMarketLevelScriptActor::AMarketLevelScriptActor() {
    PrimaryActorTick.bCanEverTick = true;
    
    BotRequest = CreateDefaultSubobject<UBotRequest>(TEXT("BotRequest"));
    BotRequest->OnResponseReceived.AddDynamic(this, &AMarketLevelScriptActor::OnBotResponseReceived);
    BotRequest->SetupAttachment(RootComponent);

    
}

void AMarketLevelScriptActor::BeginPlay() {
    Super::BeginPlay();

    if (MarketPoint) {
        MarketPoint->FillSphere->OnComponentBeginOverlap.AddDynamic(this, &AMarketLevelScriptActor::OnTargetPointOverlapBegin);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Market point is not initilized"));
    }
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

    BotRequest->Request(ECommand::NewCharacterSpawned);

    //Character->GoToMarket();
}

void AMarketLevelScriptActor::SpawnBasket() {
    if (BasketSpawnPoint == nullptr || ToBasketSpawn == nullptr)
        return;

    //BasketSpawnPoint->GetActorTransform();

    Basket = Cast<ABasket>(GetWorld()->SpawnActor(ToBasketSpawn, &BasketSpawnPoint->GetActorTransform()));
}

void AMarketLevelScriptActor::OnBotResponseReceived(EAction Action, TArray<FString> PhraseArray) {
    PlayAction(Action);
    PlayAudio(PhraseArray);
}

void AMarketLevelScriptActor::PlayAction(EAction Action) {
    switch (Action) {
    case EAction::GoToMarket: {
        Character->GoToMarket();
        break;
    }
    default: {
        break;
    }
    }
}

void AMarketLevelScriptActor::PlayAudio(TArray<FString> PhraseArray) {
    for (auto& Phrase : PhraseArray) {
        Character->PhrasesAudio->SoundQueue.Enqueue(Phrase);
    }
    Character->PhrasesAudio->StartPlayingQueue();
}

void AMarketLevelScriptActor::OnTargetPointOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;
    
    if (OtherActor && OtherActor != this) {
        AMyCharacter* CurrentCharacter = Cast<AMyCharacter>(OtherActor);
        if (CurrentCharacter && CurrentCharacter == Character) {
            BotRequest->Request(ECommand::ReachedMarket);
        }
    }
}
