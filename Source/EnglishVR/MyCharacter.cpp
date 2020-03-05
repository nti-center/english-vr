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

    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObject(TEXT("DataTable'/Game/CSV/MainDialog.MainDialog'"));
    if (DataTableObject.Succeeded()) {
        DataTable = DataTableObject.Object;
        UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTableObject(TEXT("DataTable'/Game/CSV/DataTable.DataTable'"));
    if (_DataTableObject.Succeeded()) {
        _Table = _DataTableObject.Object;
        UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
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
        int32 requestCount = 1;
        FName SoundName = "";
        FName GetPath = "";

       // Rand = FMath::RandRange(min,max);
        SoundName = SoundsName[i];

		if (SoundName == "greetings")
			Rand = FMath::RandRange(1, 7);
		else if (SoundName == "requests")
			Rand = FMath::RandRange(1, 6);
		else if (SoundName == "errors")
			Rand = FMath::RandRange(1, 3);
		else if (SoundName == "goodbye")
			Rand = FMath::RandRange(1, 4);

        //if (SoundName == "requests")
        //{
        //    GetPath = "SoundCue'/Game/Sounds/TestRandomSoundCue.TestRandomSoundCue'";
        //
        //    FString base = SoundName.ToString();
        //    base.Append(FString::FromInt(requestCount));
        //    FName ConcatName = FName(*base);
        //
        //    DialogList.Add(ConcatName, GetPath);
        //    requestCount++;
        //}
        //else 
        //{

        FString base = SoundName.ToString();
        base.Append(FString::FromInt(Rand));

        FName ConcatName = FName(*base);

        FAudioDataTableStruct* Row = _Table->FindRow<FAudioDataTableStruct>(ConcatName, ContextString, true);
        if (Row) {
            GetPath = (*Row->Path);
			if (SoundName == "requests"){
				FruitsCount.Add((*Row->FruitType), Row->FruitCount);
				UE_LOG(LogTemp, Warning, TEXT("Type =  %s Count = %d"), *Row->FruitType, Row->FruitCount);
			}
        }
        UE_LOG(LogTemp, Warning, TEXT("Key =  %s Value = %s"), *ConcatName.ToString(), *GetPath.ToString());
        DialogList.Add(SoundName, GetPath);
        //}
    }
}

void AMyCharacter::GoToMarket() {
	if (WalkingCount < ToPath.Num()){
		Cast<AAIController>(GetController())->MoveToActor(ToPath[WalkingCount], -1.f, true, true);
	}
}

void AMyCharacter::GoAway() {
    //WalkingCount = 0;

    if (!IsTmp && IsEnd) {
        if (WalkingCount < OutPath.Num())
            Cast<AAIController>(GetController())->MoveToActor(OutPath[WalkingCount], -1.f, true, true);

        IsTmp = false;
    }
}

void AMyCharacter::TakeBasket() {

    if (Basket == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Can't take basket: basket is null"));
        return;
    }

    Basket->Mesh->SetSimulatePhysics(false);
    //AttachInside
    Basket->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Basket->AttachOverlappingActors();
    FAttachmentTransformRules Atr(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
    Basket->AttachToComponent(GetMesh(), Atr, "RightHandSocket");
    PlayDialog(DialogList.FindRef("goodbye"), DataTable, IsCheck);
    IsEnd = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
    Super::BeginPlay();

    IsCheck = true;
    WalkingCount = 0;

    TArray<FName> name;
    name.Add("greetings");
    name.Add("requests");
    name.Add("errors");
    name.Add("goodbye");

    RandomDialogGenerator(name);

    GoToMarket();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    
    if (!IsNotPlaying())
        return;

    if (EComeState == EStatesEnum::Active) {
        this->PlayDialog(DialogList.FindRef("greetings"), DataTable, IsCheck);
        //this->PlayDialog("greetings4", DataTable, isCheck);
        this->PlayDialog(DialogList.FindRef("requests"), DataTable, IsCheck);
        //this->PlayDialog("requests4", DataTable, isCheck);
        EComeState = EStatesEnum::Finished;
    }
    if (EPickupState == EStatesEnum::Finished && !IsEnd) {
        TakeBasket();
    }

    GoAway();
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
        PlayDialog(DialogList.FindRef("errors"), DataTable, IsCheck);
        //this->PlayDialog("errors3", DataTable, isCheck);
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

