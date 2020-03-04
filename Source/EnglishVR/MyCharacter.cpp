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

bool AMyCharacter::IsState(EStatesEnum A, EStatesEnum B) {
    if (A == B)
        return true;
    else
        return false;
}

bool AMyCharacter::IsCorrectFruitsCount(TMap<FString, int32> _A, TMap<FString, int32> _B) {
    TArray<FString> keys;

    _A.GenerateKeyArray(keys);
    
    for (int i = 0; i < keys.Num(); i++) {
        if (_B.Contains(keys[i])) {
            int32* a = _A.Find(keys[i]);
            int32* b = _B.Find(keys[i]);

            if (a != b) {
                return false;
            }
        }
        else {
            return false;
        }
        return true;
    }
    return false;
}

void AMyCharacter::RandomDialogGenerator(TArray<FName> SoundsName, int32 min, int32 max) {
    FString ContextString;

    for (int i = 0; i < SoundsName.Num(); i++) {
        int32 Rand = 0;
        int32 requestCount = 1;
        FName SoundName = "";
        FName GetPath = "";

        Rand = FMath::RandRange(min,max);
        SoundName = SoundsName[i];

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
        }
        UE_LOG(LogTemp, Warning, TEXT("Key =  %s Value = %s"), *ConcatName.ToString(), *GetPath.ToString());
        DialogList.Add(SoundName, GetPath);
        //}
    }
}

void AMyCharacter::GoToMarket() {
    if (walkingCount < ToPath.Num())
        ai->MoveToActor(ToPath[walkingCount], -1.f, true, true);
}

void AMyCharacter::GoAway() {
    walkingCount = 0;

    if (!IsTmp && IsEnd) {
        if (walkingCount < OutPath.Num()) {
            ai->MoveToActor(OutPath[walkingCount], -1.f, true, true);
        }

        IsTmp = false;
    }
}

void AMyCharacter::GetABasket() {
    if ((EPickupState == EStatesEnum::Finished) && !IsEnd) {
        UStaticMeshComponent* _mesh = Cast<UStaticMeshComponent>(Basket);
        if (!_mesh) {
            UE_LOG(LogTemp, Warning, TEXT("Not found basket mesh"));
            return;
        }

        _mesh->SetSimulatePhysics(false);
        //Attach Insaide
        _mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        _mesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHandSocket");
        this->PlayDialog(DialogList.FindRef("goodbye"), DataTable, IsCheck);
        //this->PlayDialog("goodbye3", DataTable, isCheck);
        IsEnd = true;
    }
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
    Super::BeginPlay();

    IsCheck = true;
    walkingCount = 0;

    TArray<FName> name;
    name.Add("greetings");
    name.Add("requests");
    name.Add("errors");
    name.Add("goodbye");

    RandomDialogGenerator(name, 1, 3);

    PlayerMesh = GetMesh();
    if (PlayerMesh) {
        PlayerMesh->SetSkeletalMesh(AlternateMeshAsset);
        UE_LOG(LogTemp, Warning, TEXT("Player Mesh loaded"));
    }

    ai = Cast<AAIController>(thisCharacter->GetController());

    if (!ai) {
        UE_LOG(LogTemp, Warning, TEXT("Not found ai"));
        return;
    }

    GoToMarket();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    
    if (this->IsNotPlaying()) {
        if (EComeState == EStatesEnum::Active) {
            this->PlayDialog(DialogList.FindRef("greetings"), DataTable, IsCheck);
            //this->PlayDialog("greetings4", DataTable, isCheck);
            this->PlayDialog(DialogList.FindRef("requests"), DataTable, IsCheck);
            //this->PlayDialog("requests4", DataTable, isCheck);
            EComeState = EStatesEnum::Finished;
        }

        GoAway();
    }
}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)  {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;

    if (OtherActor->ActorHasTag("Basket")) {
        if (IsNotPlaying()) {
            if (!(EPickupState == EStatesEnum::Finished)) {
                Basket = Cast<ABasket>(OtherActor);
                if (!Basket) {
                    UE_LOG(LogTemp, Warning, TEXT("Not basket"));
                    return;
                }
                if (IsCorrectFruitsCount(FruitsCount, Basket->CountItems)) {
                    EPickupState = EStatesEnum::Active;
                }
                else {
                    this->PlayDialog(DialogList.FindRef("errors"), DataTable, IsCheck);
                    //this->PlayDialog("errors3", DataTable, isCheck);
                    ENegativeState = EStatesEnum::Active;
                }
            }
        }
    }
}

void AMyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;

    if (OtherActor && OtherActor != this) {
        if (IsNotPlaying()) {
            if (!(EPickupState == EStatesEnum::Finished)) {
                EPickupState = EStatesEnum::NotActive;
            }
        }
    }
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

