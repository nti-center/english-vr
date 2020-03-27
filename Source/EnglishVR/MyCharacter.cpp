// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

AMyCharacter::AMyCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
    Box->SetGenerateOverlapEvents(true);
    Box->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
    Box->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd);

    Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
    Audio->SetupAttachment(RootComponent);

#pragma region DataTableLoading

   //static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableObject(TEXT("DataTable'/Game/CSV/DataTable.DataTable'"));
   //if (_DataTableObject.Succeeded()) {
   //    _Table = _DataTableObject.Object;
   //    //UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
   //}

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableGreetings(TEXT("DataTable'/Game/CSV/Greetings_phrases_table.Greetings_phrases_table'"));
    if (_DataTableGreetings.Succeeded()) {
        GreetingsTable = _DataTableGreetings.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableGratitude(TEXT("DataTable'/Game/CSV/Gratitude_phrases_table.Gratitude_phrases_table'"));
    if (_DataTableGratitude.Succeeded()) {
        GratitudeTable = _DataTableGratitude.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTablePayment(TEXT("DataTable'/Game/CSV/payment_phrases_table.Payment_phrases_table'"));
    if (_DataTablePayment.Succeeded()) {
        PaymentTable = _DataTablePayment.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableGoodbye(TEXT("DataTable'/Game/CSV/Goodbye_phrases_table.Goodbye_phrases_table'"));
    if (_DataTableGoodbye.Succeeded()) {
        GoodbyeTable = _DataTableGoodbye.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableErrors(TEXT("DataTable'/Game/CSV/Error_phrases_table.Error_phrases_table'"));
    if (_DataTableErrors.Succeeded()) {
        ErrorsTable = _DataTableErrors.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableRequest(TEXT("DataTable'/Game/CSV/Request_phrases_table.Request_phrases_table'"));
    if (_DataTableRequest.Succeeded()) {
        RequestTable = _DataTableRequest.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableNumber(TEXT("DataTable'/Game/CSV/Numbers_phrases_table.Numbers_phrases_table'"));
    if (_DataTableNumber.Succeeded()) {
        NumberTable = _DataTableNumber.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableFruit(TEXT("DataTable'/Game/CSV/Fruit_phrases_table.Fruit_phrases_table'"));
    if (_DataTableFruit.Succeeded()) {
        FruitTable = _DataTableFruit.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableFruits(TEXT("DataTable'/Game/CSV/Fruits_phrases_table.Fruits_phrases_table'"));
    if (_DataTableFruits.Succeeded()) {
        FruitsTable = _DataTableFruits.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableEnding(TEXT("DataTable'/Game/CSV/Ending_phrases_table.Ending_phrases_table'"));
    if (_DataTableEnding.Succeeded()) {
        EndingTable = _DataTableEnding.Object;
    }
#pragma endregion
}

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path) {
    if (Path == NAME_None) return NULL;
    return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

bool AMyCharacter::IsCorrectFruitsCount() {
    if (FruitsCount.Num() != Basket->FruitCounts.Num())
        return false;

    for (auto& Item : FruitsCount)
        if (!Basket->FruitCounts.Contains(Item.Key) || Item.Value != Basket->FruitCounts[Item.Key])
            return false;

    return true;
}

void AMyCharacter::RandomDialogGenerator(TArray<FName> SoundsName) {

    FString ContextString;
    FString base = "";

    int32 Rand = 0;

    FName SoundName = "";
    FName GetPath = "";
    FName ConcatName = "";

    for (int i = 0; i < SoundsName.Num(); i++) {

        SoundName = SoundsName[i];

        if (SoundName == "greetings") {
            Rand = FMath::RandRange(1, 7);
            TmpTable = GreetingsTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FSoundDataTableStruct* Row = TmpTable->FindRow<FSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
            }
        }
        else if (SoundName == "gratitude") {
            Rand = FMath::RandRange(1, 6);
            TmpTable = GratitudeTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FSoundDataTableStruct* Row = TmpTable->FindRow<FSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
            }
        }
        else if (SoundName == "payment") {
            Rand = FMath::RandRange(1, 1);

            TmpTable = PaymentTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FSoundDataTableStruct* Row = TmpTable->FindRow<FSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
            }
        }
        else if (SoundName == "goodbye") {
            Rand = FMath::RandRange(1, 4);

            TmpTable = GoodbyeTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FSoundDataTableStruct* Row = TmpTable->FindRow<FSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
            }
        }
        else if (SoundName == "errors") {
            Rand = FMath::RandRange(4, 4);

            TmpTable = ErrorsTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FSoundDataTableStruct* Row = TmpTable->FindRow<FSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
            }
        }

        DialogList.Add(SoundName, GetPath);
    }
}

void AMyCharacter::RandomRequestGenerator() {

    FString ContextString = "";
    FString base = "";
    FString FruitType = "";

    int32 Rand = 0;
    int32 FruitCount;
    int32 numbers = FMath::RandRange(1, 5);

    FName SoundName = "";
    FName GetPath = "";
    FName ConcatName = "";

    TArray<FName> tmp;

    bool IsOtherFruit = false;

    //Очищаем массив с путями к звукам и TMap с количеством и типом фруктов
    RequestFullPhrasesArray.Empty();
    FruitsCount.Empty();
    
    //Заполняем массив типами звуков, которые будем искать в таблицах
    tmp.Add("request");
    tmp.Add("numbers");
    //Если фруктов больше одного ищем в таблице - Фрукты
    if (numbers> 1) {
        tmp.Add("fruits");
    }
    //Иначе в таблице - Фрукт
    else {
        tmp.Add("fruit");
    }
    tmp.Add("ending");

    for (int i = 0; i < tmp.Num(); i++) {
        SoundName = tmp[i];

        if (SoundName == "request") {
            TmpTable = RequestTable;

            FSoundDataTableStruct* Row;

            //Если это первый запрос для данного пресонажа
            if (Counter == 0) {
                //Берем все пути к звука из таблицы reques и записываем их в массив
                for (auto it : RequestTable->GetRowMap()) {
                    Row = TmpTable->FindRow<FSoundDataTableStruct>(it.Key, ContextString, true);
                    if (Row) {
                        RequestPhrasesArray.Add((*Row->Path));
                    }
                }
                //Ищем рандомный запрос в массиве и добавляем его в GetPath, после чего удаляем этот элемент из массива
                //Делаем это, что бы не повторялись звуки запроса
                RequestPhrasesArrayLength = RequestPhrasesArray.Num() - 1;
                Rand = FMath::RandRange(0, RequestPhrasesArrayLength);

                GetPath = RequestPhrasesArray[Rand];

                //RequestPhrasesArray.RemoveAt(Rand);
                //RequestPhrasesArrayLength--;
            }
            //Ищем рандомный запрос в массиве и добавляем его в GetPath, после чего удаляем этот элемент из массива
            //Делаем это, что бы не повторялись звуки запроса
            else
            {
                Rand = FMath::RandRange(0, RequestPhrasesArrayLength);

                GetPath = RequestPhrasesArray[Rand];

                //RequestPhrasesArray.RemoveAt(Rand);
               // RequestPhrasesArrayLength--;
            }
        }
        else if (SoundName == "numbers") {
            Rand = numbers;
            TmpTable = NumberTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FNumbersSoundDataTableStruct* Row = TmpTable->FindRow<FNumbersSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
                FruitCount = Row->Count;
            }
        }
        else if (SoundName == "fruit") {
            //Идем в цикле, пока не найдем тип фрукта, отличный от того, что был на предыдущем шаге
            while (IsOtherFruit == false) {
                Rand = FMath::RandRange(1, 5);
                TmpTable = FruitTable;

                base = SoundName.ToString();
                base.Append(FString::FromInt(Rand));

                ConcatName = FName(*base);

                FFruitSoundDataTableStruct* Row = TmpTable->FindRow<FFruitSoundDataTableStruct>(ConcatName, ContextString, true);
                if (Row) {
                    GetPath = (*Row->Path);
                    FruitType = *Row->FruitType;
                }
                if (FruitType != PreviousFruit) {
                    PreviousFruit = FruitType;
                    IsOtherFruit = true;
                }
            }
        }
        else if (SoundName == "fruits") {
            //Идем в цикле, пока не найдем тип фрукта, отличный от того, что был на предыдущем шаге
            while (IsOtherFruit == false) {
                Rand = FMath::RandRange(1, 3);
                TmpTable = FruitsTable;

                base = SoundName.ToString();
                base.Append(FString::FromInt(Rand));

                ConcatName = FName(*base);

                FFruitSoundDataTableStruct* Row = TmpTable->FindRow< FFruitSoundDataTableStruct>(ConcatName, ContextString, true);
                if (Row) {
                    GetPath = (*Row->Path);
                    FruitType = *Row->FruitType; 
                }
                if ((FruitType != PreviousFruit) && !((numbers > 2) && ((FruitType == "Melon") || (FruitType == "Watermelon"))) ) {
                    PreviousFruit = FruitType;
                    IsOtherFruit = true;
                }
            }
        }
        else if (SoundName == "ending") {
            if (Counter == 0)
                Rand = 1;
            else
                Rand = FMath::RandRange(1, 2);

            TmpTable = EndingTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FSoundDataTableStruct* Row = TmpTable->FindRow<FSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
            }
        }
        //Заполняем массив путей к звукам
        RequestFullPhrasesArray.Add(GetPath);
    } 
    //Заполняем TMap с типом и количеством фруктов
   FruitsCount.Add(FruitType, FruitCount);
   AllRequestsFruitsAndCountList.Add(FruitType, FruitCount);
}

void AMyCharacter::GoToMarket() {
	if (WalkingCount < ToPath.Num()) {
        if (GetController() && Cast<AAIController>(GetController()))
		    Cast<AAIController>(GetController())->MoveToActor(ToPath[WalkingCount], -1.f, true, true);
	}
}

void AMyCharacter::GoAway() {

    if (WalkingCount < OutPath.Num())
        if (GetController() && Cast<AAIController>(GetController()))
            Cast<AAIController>(GetController())->MoveToActor(OutPath[WalkingCount], -1.f, true, true);

    IsTmp = false;
}

void AMyCharacter::TakeBasket() {

    if (Basket == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Can't take basket: basket is null"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("TakeBasket"));

    Basket->Mesh->SetSimulatePhysics(false);
    Basket->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Basket->AttachOverlappingActors();
    FAttachmentTransformRules Atr(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
    Basket->AttachToComponent(GetMesh(), Atr, "RightHandSocket");
    PlayDialog(DialogList.FindRef("gratitude"), IsCheck);
    PlayDialog(DialogList.FindRef("goodbye"), IsCheck);
    IsEnd = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
    Super::BeginPlay();

    IsCheck = true;
    WalkingCount = 0;

    RequestCount = 0;
    RequestCount = 2;//FMath::RandRange(2,3);

    Counter = 0;

    TArray<FName> name;
    name.Add("greetings");
    name.Add("gratitude");
    name.Add("payment");
    name.Add("goodbye");
	name.Add("errors");

   RandomDialogGenerator(name);
   GoToMarket();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (!IsNotPlaying())
        return;

    if (EComeState == EStatesEnum::Active) {
        PlayDialog(DialogList.FindRef("greetings"), IsCheck);

        EComeState = EStatesEnum::Process;
        //EComeState = EStatesEnum::Finished;
    }

	if((EComeState == EStatesEnum::Process) && (Counter < RequestCount)){

        RandomRequestGenerator();
        PlayRequestList(RequestFullPhrasesArray, RequestFullPhrasesArray.Num(), IsCheck);

        for (TActorIterator<ABasket> ActorItr(GetWorld()); ActorItr; ++ActorItr)
        {
            Basket = Cast<ABasket>(*ActorItr);
        }    
        EComeState = EStatesEnum::Whaiting;
	}
    else if ((Basket) && (EComeState == EStatesEnum::Whaiting)) {
        if (IsCorrectFruitsCount()) {
            
            //UE_LOG(LogTemp, Warning, TEXT("CorrectFruitCount"));

            FruitsCount.Empty();
            Basket->FruitCounts.Empty();

            Counter++;
            EComeState = EStatesEnum::Process;
        }
    }
    else if (Counter == RequestCount) {
        if(EComeState != EStatesEnum::Finished)
            PlayDialog(DialogList.FindRef("payment"), IsCheck);
        EComeState = EStatesEnum::Finished;
    }

    if (EPickupState == EStatesEnum::Finished && !IsEnd) {
        TakeBasket();
    }

    if (!IsTmp && IsEnd && IsNotPlaying()) {
        GoAway();
    }
}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)  {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;
    if (!IsNotPlaying() || EPickupState == EStatesEnum::Finished || !Cast<ABasket>(OtherActor))
        return;

    int32 count = 0;

    Basket = Cast<ABasket>(OtherActor);

    for (auto& Elem : AllRequestsFruitsAndCountList){
        UE_LOG(LogTemp, Warning, TEXT("Key %s value %d"), *Elem.Key, Elem.Value);
        for (auto& Elem_2 : Basket->AllFruitCounts){

            if ((Elem.Key == Elem_2.Key) && (Elem.Value == Elem_2.Value)) {
                count++;
            }
            UE_LOG(LogTemp, Warning, TEXT("Basket Key %s Basket value %d"), *Elem.Key, Elem.Value);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Count %d Num %d"), count, AllRequestsFruitsAndCountList.Num());

   if (count == AllRequestsFruitsAndCountList.Num()) {
       EPickupState = EStatesEnum::Active;   
   }
   else {
       PlayDialog(DialogList.FindRef("errors"), IsCheck);
       PlayRequestList(RequestFullPhrasesArray, RequestFullPhrasesArray.Num(), IsCheck);
   
       ENegativeState = EStatesEnum::Active;
   }
}

void AMyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;

    if (!IsNotPlaying() || EPickupState == EStatesEnum::Finished || !Cast<ABasket>(OtherActor))
        return;

    EPickupState = EStatesEnum::NotActive;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

