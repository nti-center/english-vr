// Fill out your copyright notice in the Description page of Project Settings.


#include "MarketLevelScriptActor.h"

AMarketLevelScriptActor::AMarketLevelScriptActor() {
    PrimaryActorTick.bCanEverTick = true;

    BotRequest = CreateDefaultSubobject<UBotRequest>(TEXT("BotRequest"));
    BotRequest->OnResponseReceived.AddDynamic(this, &AMarketLevelScriptActor::OnBotResponseReceived);
    BotRequest->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTable(TEXT("DataTable'/Game/CSV/CharacterparametrsTable.CharacterparametrsTable'"));
    if (_DataTable.Succeeded()) {
        CharacterParametrsTable = _DataTable.Object;
    }

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

    FruitController->SpawnFruits();
    
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

    BotRequest->Request(ECommand::MarketLevelStarted);
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

    //UE_LOG(LogTemp, Warning, TEXT("Num: %d Rand: %d"), SpawnPointsArray.Num(), Rand);

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
    TArray <FName>  OutRowNames;
    FActorSpawnParameters SpawnParametrs;
    FString StringPath = "";
    FString ContextString;

    SelectSpawnAndDestroyPoint();

    UDataTableFunctionLibrary::GetDataTableRowNames(CharacterParametrsTable,OutRowNames);
    int32 Rand = FMath::RandRange(1, 1/*OutRowNames.Num()*/);
    FCharacterDataTableStruct* Row = CharacterParametrsTable->FindRow<FCharacterDataTableStruct>(OutRowNames[Rand-1], ContextString, true);
    if (Row) {
        StringPath = *Row->PathToBP; 
        SpawnParametrs.Name = OutRowNames[Rand - 1];
    }

    const TCHAR *Path = *StringPath;
    ToCharacterSpawn = LoadClass<AMyCharacter>(nullptr, Path);

    if (CharacterSpawnPoint == nullptr || ToCharacterSpawn == nullptr)
        return;
    Character = Cast<AMyCharacter>(GetWorld()->SpawnActor(ToCharacterSpawn, &CharacterSpawnPoint->GetActorTransform(), SpawnParametrs));

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


void AMarketLevelScriptActor::OnSpeechRecognized(FString Text) {
    UE_LOG(LogTemp, Warning, TEXT("Recognized text: %s"), *Text);
    BotRequest->Request(ECommand::SpeechRecognized, TArray<FString>({Text.Replace(TEXT(" "), TEXT("+"))}));
}

void AMarketLevelScriptActor::OnCharacterCanTakeBasket() {
    BotRequest->Request(ECommand::CanTakeBasket);
}

void AMarketLevelScriptActor::OnBotResponseReceived(FString ResponseString) {
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ResponseString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
        TSharedPtr<FJsonObject> Response = JsonObject->GetObjectField("response");
        UE_LOG(LogTemp, Warning, TEXT("Bot answer is: %s"), *Response->GetStringField("text"));
        FString TextString = Response->GetStringField("text");
        TextString.RemoveFromEnd(".");
        Reader = TJsonReaderFactory<TCHAR>::Create(TextString);
        if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
            UE_LOG(LogTemp, Warning, TEXT("Actions is: %s"), *JsonObject->GetStringField("Actions"));
            UE_LOG(LogTemp, Warning, TEXT("VoicePhrases is: %s"), *JsonObject->GetStringField("VoicePhrases"));

            EAction Action = Action = EAction::None;
            if (Actions.Contains(JsonObject->GetStringField("Actions"))) {
                Action = Actions[JsonObject->GetStringField("Actions")];
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Undefine action: %s"), *JsonObject->GetStringField("Actions"));
            }

            TArray<TSharedPtr<FJsonValue>> Params;
            if (JsonObject->HasField("Params"))
                Params = TArray<TSharedPtr<FJsonValue>>(JsonObject->GetArrayField("Params"));

            if (Action != EAction::None)
                PlayAction(Action, Params);

            if (Character != nullptr)
                PlayAudio(ParsePhrasesString(JsonObject->GetStringField("VoicePhrases")), Widget);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Cant deserialize text"));
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Cant deserialize response"));
    }
}

TArray<FString> AMarketLevelScriptActor::ParsePhrasesString(const FString& PhrasesString) {
    TArray<FString> PhraseArray;
    PhrasesString.ParseIntoArray(PhraseArray, TEXT(" "), true);
    return PhraseArray;
}

void AMarketLevelScriptActor::SetProductPrices(TArray<TSharedPtr<FJsonValue>> ParamArray) {
    ProductPrices.Empty();
    for (auto& Value : ParamArray) {
        FString Product = Value.Get()->AsArray()[0].Get()->AsArray()[1].Get()->AsString();
        int32 Price = FCString::Atoi(*Value.Get()->AsArray()[1].Get()->AsArray()[1].Get()->AsString());
        ProductPrices.Add(Product, Price);
    }
}

void AMarketLevelScriptActor::PlayAction(EAction Action, TArray<TSharedPtr<FJsonValue>> ParamArray) {
    switch (Action) {
    case EAction::GoToMarket: {
        Character->SetPath(ToPath);
        Character->Go();
        break;
    }
    case EAction::SetRequest: {
        Character->ClearFruitRequests();
        for (int i = 0; i < ParamArray.Num(); i += 2)
            Character->AddFruitRequest(ParamArray[i + 1].Get()->AsString(), FCString::Atoi(*ParamArray[i].Get()->AsString()));
        break;
    }
    case EAction::Hide: {
        Character->AnimationState = EAnimationState::Hiding;
        break;
    }
    case EAction::AddRequest: {
        for (int i = 0; i < ParamArray.Num(); i += 2)
            Character->AddFruitRequest(ParamArray[i + 1].Get()->AsString(), FCString::Atoi(*ParamArray[i].Get()->AsString()));
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
    case EAction::SetProductPrices: {
        SetProductPrices(ParamArray);
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
    Character->PhrasesAudio->PlaySoundWithCrossfade(PhraseArray,TextWidget,Character->GetName());
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
