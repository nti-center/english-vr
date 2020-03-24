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

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableObject(TEXT("DataTable'/Game/CSV/DataTable.DataTable'"));
    if (_DataTableObject.Succeeded()) {
        _Table = _DataTableObject.Object;
        //UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableRequest(TEXT("DataTable'/Game/CSV/Request_phrases_table.Request_phrases_table'"));
    if (_DataTableRequest.Succeeded()) {
        RequestTable = _DataTableRequest.Object;
        //UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableNumber(TEXT("DataTable'/Game/CSV/Numbers_phrases_table.Numbers_phrases_table'"));
    if (_DataTableNumber.Succeeded()) {
        NumberTable = _DataTableNumber.Object;
        //UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableFruit(TEXT("DataTable'/Game/CSV/Fruit_phrases_table.Fruit_phrases_table'"));
    if (_DataTableFruit.Succeeded()) {
        FruitTable = _DataTableFruit.Object;
        //UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableFruits(TEXT("DataTable'/Game/CSV/Fruits_phrases_table.Fruits_phrases_table'"));
    if (_DataTableFruits.Succeeded()) {
        FruitsTable = _DataTableFruits.Object;
        //UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableEnding(TEXT("DataTable'/Game/CSV/Ending_phrases_table.Ending_phrases_table'"));
    if (_DataTableEnding.Succeeded()) {
        EndingTable = _DataTableEnding.Object;
       // UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
    }
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

    for (int i = 0; i < SoundsName.Num(); i++) {
        int32 Rand = 0;
        FName SoundName = "";
        FName GetPath = "";

       // Rand = FMath::RandRange(min,max);
        SoundName = SoundsName[i];

		if (SoundName == "greetings")
			Rand = FMath::RandRange(1, 7);
		//else if (SoundName == "requests")
		//	Rand = FMath::RandRange(1, 6);
        else if (SoundName == "gratitude")
            Rand = FMath::RandRange(1, 6);
		else if (SoundName == "goodbye")
			Rand = FMath::RandRange(1, 4);
		else if (SoundName == "errors")
			Rand = FMath::RandRange(3, 3);

        FString base = SoundName.ToString();
        base.Append(FString::FromInt(Rand));

        FName ConcatName = FName(*base);

        FAudioDataTableStruct* Row = _Table->FindRow<FAudioDataTableStruct>(ConcatName, ContextString, true);
        if (Row) {
            GetPath = (*Row->Path);
			if (SoundName == "requests"){
                RandomRequestGenerator();
				//FruitsCount.Add((*Row->FruitType), Row->FruitCount);
				//UE_LOG(LogTemp, Warning, TEXT("Type =  %s Count = %d"), *Row->FruitType, Row->FruitCount);
			}
        }
        //UE_LOG(LogTemp, Warning, TEXT("Key =  %s Value = %s"), *ConcatName.ToString(), *GetPath.ToString());
        if(SoundName != "requests")
        DialogList.Add(SoundName, GetPath);
    }
}

void AMyCharacter::RandomRequestGenerator() {

    RequestPhrasesList.Empty();
    FruitsCount.Empty();

    FString ContextString;
    int32 Rand = 0;
    FName SoundName = "";
    FName GetPath = "";
    TArray<FName> tmp;

    FString FruitType;
    int32 FruitCount;

    tmp.Add("request");
    tmp.Add("numbers");

    int32 numbers = FMath::RandRange(1, 5);

    if (numbers> 1) {
        tmp.Add("fruits");
    }
    else {
        tmp.Add("fruit");
    }
    tmp.Add("ending");

    FString base;
    FName ConcatName;

    for (int i = 0; i < 4; i++) {

        SoundName = tmp[i];

        if (SoundName == "request") {
            Rand = FMath::RandRange(1, 2);
            TmpTable = RequestTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FSoundDataTableStruct* Row = TmpTable->FindRow<FSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
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
            Rand = FMath::RandRange(1, 5);
            TmpTable = FruitTable;

            base = SoundName.ToString();
            base.Append(FString::FromInt(Rand));

            ConcatName = FName(*base);

            FFruitSoundDataTableStruct* Row = TmpTable->FindRow< FFruitSoundDataTableStruct>(ConcatName, ContextString, true);
            if (Row) {
                GetPath = (*Row->Path);
                FruitType = *Row->FruitType;
            }
        }
        else if (SoundName == "fruits") {
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
        }
        else if (SoundName == "ending") {
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

        RequestPhrasesList.Add(GetPath);
        UE_LOG(LogTemp, Warning, TEXT("RequestPhrases %s"), *RequestPhrasesList[i].ToString());
    } 

   FruitsCount.Add(FruitType, FruitCount);

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

    TArray<FName> name;
    name.Add("greetings");
    name.Add("gratitude");
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

        RandomRequestGenerator();
        PlayRequestList(RequestPhrasesList, IsCheck);

        //PlayDialog(DialogList.FindRef("requests"), IsCheck);
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

    Basket = Cast<ABasket>(OtherActor);

    if (IsCorrectFruitsCount()) {
        EPickupState = EStatesEnum::Active;
    }
    else {
        PlayDialog(DialogList.FindRef("errors"), IsCheck);

        PlayRequestList(RequestPhrasesList, IsCheck);

        //PlayDialog(DialogList.FindRef("requests"), IsCheck);
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

