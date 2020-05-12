// Fill out your copyright notice in the Description page of Project Settings.


#include "MarketLevelScriptActor.h"

AMarketLevelScriptActor::AMarketLevelScriptActor() {
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTable(TEXT("DataTable'/Game/CSV/Fruits.Fruits'"));
    if (_DataTable.Succeeded()) {
        DataTable = _DataTable.Object;
    }

    FruitClass = AFruit::StaticClass();

    BotRequest = CreateDefaultSubobject<UBotRequest>(TEXT("BotRequest"));
    BotRequest->OnResponseReceived.AddDynamic(this, &AMarketLevelScriptActor::OnBotResponseReceived);
    BotRequest->SetupAttachment(RootComponent);

    SpeechRecognizerType = ASpeechRecognizerMCSS::StaticClass();
}

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path) {
    if (Path == NAME_None) return NULL;
    return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

void AMarketLevelScriptActor::BeginPlay() {
    Super::BeginPlay();

    if (SpeechRecognizerType != nullptr) {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.Instigator = GetInstigator();
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnInfo.OverrideLevel = GetLevel();
        SpawnInfo.ObjectFlags |= RF_Transient;
        SpeechRecognizer = GetWorld()->SpawnActor<ASpeechRecognizer>(SpeechRecognizerType, GetActorLocation(), GetActorRotation(), SpawnInfo);
        if (SpeechRecognizer != nullptr) {
            SpeechRecognizer->OnRecognized.AddDynamic(this, &AMarketLevelScriptActor::OnSpeechRecognized);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Cant spawn speech recognizer"));
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Speech recognizer type is not initilized"));
    }

    //SpawnFruits();
    if (VRPawn) {
        VRPawn->OnAudioRecorded.AddDynamic(this, &AMarketLevelScriptActor::OnAudioRecorded);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("VR pawn is not initilized"));
    }

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

void AMarketLevelScriptActor::SelectSpawnAndDestroyPoint() {
    ToPath.Empty();
    OutPath.Empty();
    TArray<AActor*> SpawnPointsArray;
    TArray<AActor*> DestroyPointsArray;
    TArray<AActor*> ChildActors;

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "SpawnPoint", SpawnPointsArray);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "EndPoint", DestroyPointsArray);

    int32 Rand = FMath::RandRange(0, SpawnPointsArray.Num() - 1);

    UE_LOG(LogTemp, Warning, TEXT("Num: %d Rand: %d"), SpawnPointsArray.Num(), Rand);

    CharacterSpawnPoint = SpawnPointsArray[Rand];
    CharacterSpawnPoint->GetAllChildActors(ChildActors);

    if (ChildActors.Num() > 0) {
        ToPath.Append(ChildActors);
        ChildActors.Empty();
    }

    MarketPoint->GetAllChildActors(ChildActors);
    if (ChildActors.Num() > 0) {
        ToPath.Append(ChildActors);
        ChildActors.Empty();
    }
    ToPath.Add(MarketPoint);

    Rand = FMath::RandRange(0, DestroyPointsArray.Num() - 1);
    DestroyPointsArray[Rand]->GetAllChildActors(ChildActors);
    if (ChildActors.Num() > 0) {
        OutPath.Append(ChildActors);
        ChildActors.Empty();
    }   
    OutPath.Add(DestroyPointsArray[Rand]);
}

void AMarketLevelScriptActor::SpawnCharacter() {

    SelectSpawnAndDestroyPoint();

    if (CharacterSpawnPoint == nullptr || ToCharacterSpawn == nullptr)
        return;

    Character = Cast<AMyCharacter>(GetWorld()->SpawnActor(ToCharacterSpawn, &CharacterSpawnPoint->GetActorTransform()));

    if (Character == nullptr)
        return;

    Character->ToPath = ToPath;
    Character->OutPath = OutPath;
    Character->Box->OnComponentBeginOverlap.AddDynamic(this, &AMarketLevelScriptActor::OnPickupBoxOverlapBegin);
    Character->Box->OnComponentEndOverlap.AddDynamic(this, &AMarketLevelScriptActor::OnPickupBoxOverlapEnd);
    Character->OnCanTakeBasket.AddDynamic(this, &AMarketLevelScriptActor::OnCharacterCanTakeBasket);
    Character->PhrasesAudio->OnAudioFinished.AddDynamic(this, &AMarketLevelScriptActor::OnCharacterAudioFinished);
    Character->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMarketLevelScriptActor::OnCharacterHit);

    BotRequest->Request(ECommand::NewCharacterSpawned);

    Widget = Character->WidgetComponent;

    //Character->GoToMarket();
}

void AMarketLevelScriptActor::SpawnBasket() {
    if (BasketSpawnPoint == nullptr || ToBasketSpawn == nullptr)
        return;

    //BasketSpawnPoint->GetActorTransform();

    Basket = Cast<ABasket>(GetWorld()->SpawnActor(ToBasketSpawn, &BasketSpawnPoint->GetActorTransform()));
    Basket->OnFruitAdded.AddDynamic(this, &AMarketLevelScriptActor::OnBasketFruitCountChanged);
    Basket->OnFruitRemoved.AddDynamic(this, &AMarketLevelScriptActor::OnBasketFruitCountChanged);
}

void AMarketLevelScriptActor::OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr || !Cast<AFruit>(OtherActor))
        return;

    if (NormalImpulse.Size() > 50) 
        BotRequest->Request(ECommand::Hit);
}

void AMarketLevelScriptActor::OnAudioRecorded(FString File) {
    SpeechRecognizer->Recognize(File);
}

void AMarketLevelScriptActor::OnCharacterAudioFinished() {
    BotRequest->Request(ECommand::AudioFinished);
}

void AMarketLevelScriptActor::OnBasketFruitCountChanged() {
    if (IsCorrectFruitsCount())
        BotRequest->Request(ECommand::CorrectFruitsCount);
    else
        BotRequest->Request(ECommand::IncorrectFruitsCount);
}

void AMarketLevelScriptActor::SpawnFruits() {
    TArray<UStaticMeshComponent*> children;
    FString ContextString;
    UWorld* World = GetWorld();

    for (auto it : DataTable->GetRowMap()) {
        FAudioDataTableStruct* Row = DataTable->FindRow<FAudioDataTableStruct>(it.Key, ContextString, true);
        if (Row) {
            FruitType.Add(Row->FruitType);
            FruitPath.Add(Row->Path);
        }
    }

    MarketActor->GetComponents<UStaticMeshComponent>(children);

   // for (int i = 0; i < children.Num(); i++) {
   //     //UE_LOG(LogTemp, Error, TEXT("Children: %s"), *children[i]->GetRelativeLocation().ToString());
   // }

    if (children.Num() < 0)
        return;

    //for (auto& it : children) {
    for(int l = 0; l < 2; l++) {
        TArray<FString> tmp = RandomFruitGeneration();
        UStaticMesh* CurrentFruitMesh = LoadObjFromPath<UStaticMesh>(FName(*tmp[0]));
        FString CurrentFruitType = tmp[1];

        if (!CurrentFruitMesh) {
            UE_LOG(LogTemp, Warning, TEXT("Cant find Mesh"));
            return;
        }

        if (!FruitClass) {
            UE_LOG(LogTemp, Warning, TEXT("Cant find fruit class"));
            return;
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector FruitBoxBE = children[l]->GetStaticMesh()->GetBounds().BoxExtent;
        FVector FruitBoxOrigin = children[l]->GetComponentLocation() - FruitBoxBE;

        UE_LOG(LogTemp, Error, TEXT("Location is: %s"), *children[l]->GetComponentLocation().ToString());

        FVector FruitBE = CurrentFruitMesh->GetBounds().BoxExtent;
        FVector FruitOffset = FruitBE / 3.0f;

        FVector MaxSize = FruitBoxBE * 2;
        FIntVector Count(MaxSize / (FruitBE * 2));
        FVector OriginOffset = FruitBoxBE - FVector(Count) * FruitBE;

        for (int i = 0;; i++) {
            float ROffsetX = FMath::RandRange(-FruitOffset.X, FruitOffset.X);
            float CurrX = FruitBE.X + FruitBE.X * 2 * i;
            if (CurrX + FruitBE.X + ROffsetX > MaxSize.X)
                break;

            for (int j = 0;; j++) {
                float ROffsetY = FMath::RandRange(-FruitOffset.Y, FruitOffset.Y);
                float CurrY = FruitBE.Y + FruitBE.Y * 2 * j;
                if (CurrY + FruitBE.Y + ROffsetY > MaxSize.Y)
                    break;

                for (int k = 0;; k++) {
                    float ROffsetZ = FMath::RandRange(-FruitOffset.Z, FruitOffset.Z);
                    float CurrZ = FruitBE.Z + FruitBE.Z * 2 * k;
                    if (CurrZ + FruitBE.Z + ROffsetZ > MaxSize.Z)
                        break;

                    FRotator RRotator(FMath::RandRange(-40, 40), FMath::RandRange(-40, 40), FMath::RandRange(-40, 40));
                    FVector FruitLocation = FVector(CurrX + ROffsetX, CurrY + ROffsetY, CurrZ + ROffsetZ);
                    FVector Location = children[l]->GetComponentLocation() + OriginOffset + children[l]->GetComponentRotation().RotateVector(FruitLocation - FruitBoxBE);

                    AFruit* Fruit = World->SpawnActor<AFruit>(FruitClass, Location, RRotator, SpawnParams);

                    if (Fruit) {
                        Fruit->Mesh->SetStaticMesh(CurrentFruitMesh);
                        Fruit->Type = CurrentFruitType;
                    }
                }
            }
        }
    }
}

TArray<FString> AMarketLevelScriptActor::RandomFruitGeneration()
{
    TArray<FString> tmp;
    int32 Rand = FMath::RandRange(0, FruitType.Num() - 1);

    counter++;

    if (!AllFruits.Contains(FruitPath[Rand])) {
        AllFruits.Add(FruitPath[Rand], 1);

        tmp.Add(FruitPath[Rand]);
        tmp.Add(FruitType[Rand]);
        UE_LOG(LogTemp, Warning, TEXT("Mesh ¹ %d is %s %s"), counter, *tmp[0], *tmp[1]);
        return tmp;
    }

    int32 count = AllFruits.FindRef(FruitPath[Rand]);
    if (count >= 2)
        return tmp = RandomFruitGeneration();

    AllFruits.Add(FruitPath[Rand], 2);

    tmp.Add(FruitPath[Rand]);
    tmp.Add(FruitType[Rand]);
    UE_LOG(LogTemp, Warning, TEXT("Mesh%d is %s %s"), counter, *tmp[0], *tmp[1]);

    return tmp;
}

void AMarketLevelScriptActor::OnSpeechRecognized(FString Text) {
    UE_LOG(LogTemp, Warning, TEXT("Recognized text: %s"), *Text);
    BotRequest->Request(ECommand::SpeechRecognized, TArray<FString>({Text.Replace(TEXT(" "), TEXT("+"))}));
}

void AMarketLevelScriptActor::OnCharacterCanTakeBasket() {
    BotRequest->Request(ECommand::CanTakeBasket);
}

void AMarketLevelScriptActor::OnBotResponseReceived(EAction Action, TArray<FString> ParamArray, TArray<FString> PhraseArray) {
    PlayAction(Action, ParamArray);
    PlayAudio(PhraseArray, Widget);
}

void AMarketLevelScriptActor::PlayAction(EAction Action, TArray<FString> ParamArray) {
    switch (Action) {
    case EAction::GoToMarket: {
        Character->SetPath(ToPath);
        Character->Go();
        break;
    }
    case EAction::SetRequest: {
        Character->ClearFruitRequests();
        for (int i = 0; i < ParamArray.Num(); i += 2)
            Character->AddFruitRequest(ParamArray[i + 1], FCString::Atoi(*ParamArray[i]));
        break;
    }
    case EAction::Hide: {
        Character->AnimationState = EAnimationState::Hiding;
        break;
    }
    case EAction::AddRequest: {
        for (int i = 0; i < ParamArray.Num(); i += 2)
            Character->AddFruitRequest(ParamArray[i + 1], FCString::Atoi(*ParamArray[i]));
        break;
    }
    case EAction::TryToTakeBasket: {
        Character->AnimationState = EAnimationState::Taking;
        break;
    }
    case EAction::StopTryingToTakeBasket: {
        Character->AnimationState = EAnimationState::None;
        break;
    }
    case EAction::StartDisappointing: {
        Character->AnimationState = EAnimationState::Disappointing;
        break;
    }
    case EAction::CheckFruitsCount: {
        if (IsCorrectFruitsCount())
            BotRequest->Request(ECommand::CorrectFruitsCount);
        else
            BotRequest->Request(ECommand::IncorrectFruitsCount);
        break;
    }
    case EAction::TakeBasket: {
        if (Character->TakeBasket(Basket))
            BotRequest->Request(ECommand::BasketTaken);
        break;
    }
    case EAction::GoToHome: {
        Character->SetPath(OutPath);
        Character->Go();
        break;
    }
    default: {
        break;
    }
    }
}

bool AMarketLevelScriptActor::IsCorrectFruitsCount() {
    if (Character->FruitsCount.Num() != Basket->FruitCounts.Num())
        return false;

    for (auto& Item : Character->FruitsCount)
        if (!Basket->FruitCounts.Contains(Item.Key) || Item.Value != Basket->FruitCounts[Item.Key])
            return false;

    return true;
}

void AMarketLevelScriptActor::PlayAudio(TArray<FString> PhraseArray, UWidgetComponent* TextWidget) {
    //for (auto& Phrase : PhraseArray) {
    //    Character->PhrasesAudio->SoundQueue.Enqueue(Phrase); 
    //}
    //Character->PhrasesAudio->StartPlayingQueue();
    Character->PhrasesAudio->PlaySoundWithCrossfade(PhraseArray,TextWidget);
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

void AMarketLevelScriptActor::OnPickupBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr || !Cast<ABasket>(OtherActor) || Cast<ABasket>(OtherActor)->Mesh != OtherComp)
        return;
    
    BotRequest->Request(ECommand::BasketOverlapCharacterBegin);
}

void AMarketLevelScriptActor::OnPickupBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr || !Cast<ABasket>(OtherActor) || Cast<ABasket>(OtherActor)->Mesh != OtherComp)
        return;

    BotRequest->Request(ECommand::BasketOverlapCharacterEnd);
}
