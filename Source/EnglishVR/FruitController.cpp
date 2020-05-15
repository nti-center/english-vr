

#include "FruitController.h"
#include "AudioDataTableStruct.h"

AFruitController::AFruitController() {
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTable(TEXT("DataTable'/Game/CSV/Fruits.Fruits'"));
    if (_DataTable.Succeeded()) {
        DataTable = _DataTable.Object;
    }

    FruitClass = AFruit::StaticClass();

}

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path) {
    if (Path == NAME_None) return NULL;
    return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}


void AFruitController::BeginPlay() {
	Super::BeginPlay();
}

void AFruitController::SpawnFruits() {
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

    if (!MarketActor)
        return;

    MarketActor->GetComponents<UStaticMeshComponent>(children);

    if (children.Num() < 0)
        return;

    if (RequestedFruits.Num() == 0) {
        for (int l = 0; l < children.Num() - 1; l++) {
            RandomFruitGeneration();
            CreateFruit(children[l]);
        }
    }
    else {
    }

    if (World) {
        World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &AFruitController::DestroyFallenFruits, 5.0f, false);
    }   
}

void AFruitController::CreateFruit(UStaticMeshComponent* Box) {
    UStaticMesh* CurrentFruitMesh = LoadObjFromPath<UStaticMesh>(FName(*RandomFruitPath));
    FString CurrentFruitType = RandomFruitType;

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

    FVector FruitBoxBE = Box->GetStaticMesh()->GetBounds().BoxExtent;
    if (CurrentFruitType == "Melon" || CurrentFruitType == "Watermelon")
        FruitBoxBE += FVector(0, 10, 0);
    FVector FruitBoxOrigin = Box->Bounds.Origin - FruitBoxBE;

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

                FRotator RRotator(0, 0, 0);
                if (CurrentFruitType == "Carrot" || CurrentFruitType == "Cucumber") {
                    RRotator = FRotator(70, 0, 0);
                }
                FVector FruitLocation = FVector(CurrX + ROffsetX, CurrY + ROffsetY, CurrZ + ROffsetZ);
                FVector Location = Box->Bounds.Origin + OriginOffset + Box->GetComponentRotation().RotateVector(FruitLocation - FruitBoxBE);
                AFruit* Fruit = GetWorld()->SpawnActor<AFruit>(FruitClass, Location, RRotator, SpawnParams);

                if (Fruit) {
                    Fruit->Mesh->SetStaticMesh(CurrentFruitMesh);
                    Fruit->Type = CurrentFruitType;
                }
            }
        }
    }
}

void AFruitController::RandomFruitGeneration() {
    RandomFruitPath = "";
    RandomFruitType = "";
    int32 Rand = FMath::RandRange(0, FruitType.Num() - 1);

    if (!AllFruits.Contains(FruitPath[Rand])) {
        AllFruits.Add(FruitPath[Rand], 1);

        RandomFruitPath = FruitPath[Rand];
        RandomFruitType = FruitType[Rand];
        return;
    }

    int32 count = AllFruits.FindRef(FruitPath[Rand]);
    if (count >= 2)
        return RandomFruitGeneration();

    AllFruits.Add(FruitPath[Rand], 2);

    RandomFruitPath = FruitPath[Rand];
    RandomFruitType = FruitType[Rand];

    return;
}

void AFruitController::DestroyFallenFruits() {
    TArray<AActor*> FruitArray;

    TSubclassOf<AFruit> ClassToFind;
    ClassToFind = AFruit::StaticClass();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FruitArray);

    for (auto& fruit : FruitArray) {
        FVector location = fruit->GetActorLocation();
        if (location.Z < 170) {
            fruit->Destroy();
        }
    }
}

void AFruitController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

