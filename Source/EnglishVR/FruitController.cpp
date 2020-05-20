

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
    Counter = 0;
}

void AFruitController::SpawnFruits(TArray<FString> RequestedFruits, TArray<FString> Boxes, bool RandomSpawn) {
    TArray<UStaticMeshComponent*> children;
    FString ContextString;
    UWorld* World = GetWorld();

    for (auto it : DataTable->GetRowMap()) {
        FAudioDataTableStruct* Row = DataTable->FindRow<FAudioDataTableStruct>(it.Key, ContextString, true);
        if (Row) {
            FruitType.Add(Row->FruitType);
            FruitPath.Add(Row->Path);

            TypeAndPath.Add(Row->FruitType, Row->Path);
            TypeAndClass.Add(Row->FruitType, Row->VegetablesOrFruits);
        }
    }

    if (!MarketActor)
        return;

    MarketActor->GetComponents<UStaticMeshComponent>(children);

    if (children.Num() < 0)
        return;

    for (int i = 0; i < children.Num(); i++) { 
        if (children[i]->GetName() == "StaticMesh")
            children.RemoveAt(i, 1);
    }
    for (int i = 0; i < children.Num() - 1; i++) {
        for (int j = 0; j < children.Num() - i - 1; j++) {
            if (FCString::Atoi(*UKismetStringLibrary::GetSubstring(children[j]->GetName(), 3, children[j]->GetName().Len())) >
                FCString::Atoi(*UKismetStringLibrary::GetSubstring(children[j + 1]->GetName(), 3, children[j + 1]->GetName().Len()))) {
                UStaticMeshComponent* tmp = children[j];
                children[j] = children[j + 1];
                children[j + 1] = tmp;
            }
        }
    }

    TArray< UStaticMeshComponent*> Tmp;
    if ((RequestedFruits.Num() != 0 && Boxes.Num() != 0 && RandomSpawn == false) || (RequestedFruits.Num()==0 && Boxes.Num() != 0 && RandomSpawn == true)) {
        for (int i = 0; i < children.Num(); i++) {
            for (int j = 0; j < Boxes.Num(); j++) {
                if (children[i]->GetName() == Boxes[j]) {
                    Tmp.Add(children[i]);
                    break;
                }
            }
        }
        children.Empty();
        children.Append(Tmp);
    }

    //Only for Makrete scene!------------------
    if (RandomSpawn == true) {
        Tmp.Empty();
        int32 length = children.Num() / 2;
        for (int i = 0; i < length; i += 2) {
             Tmp.Add(children[i]);
             Tmp.Add(children[i + 1]);
    
            children.RemoveAt(i, 2);
        }
        Tmp.Append(children);
        children.Empty();
        children.Append(Tmp);
    }
    //-----------------------------------------

    //If we have only random || we have boxes and random
    if (RequestedFruits.Num() == 0 && RandomSpawn == true) {
        for (int l = 0; l < children.Num(); l++) {
            RandomFruitGeneration(children.Num());
            CreateFruit(children[l], RandomFruitPath, RandomFruitType);
        }
    }
    //If we have fruits,boxes and random
    else if(RequestedFruits.Num() != 0 && Boxes.Num() != 0 && RandomSpawn == true){
        for (int i = 0; i < RequestedFruits.Num(); i++) {
            for (int j = 0; j < children.Num(); j++) {
                if (children[j]->GetName() == Boxes[i]) {
                    CreateFruit(children[j], TypeAndPath.FindRef(RequestedFruits[i]), RequestedFruits[i]);

                    children.RemoveAt(j, 1);
                }
            }
            int32 Index = 0;
            FruitType.Find(RequestedFruits[i], Index);
            AllFruits.Add(FruitPath[Index], 1);
        }
        if (RandomSpawn == true) {
            for (int l = 0; l < children.Num(); l++) {
                RandomFruitGeneration(children.Num());
                CreateFruit(children[l], RandomFruitPath, RandomFruitType);
            }
        }
    }
    //If we have fruit and boxes
    else if (RequestedFruits.Num() != 0 && Boxes.Num() != 0 && RandomSpawn == false) {
        for (int i = 0; i < RequestedFruits.Num(); i++) {
            for (int j = 0; j < children.Num(); j++) {
                    CreateFruit(children[j], TypeAndPath.FindRef(RequestedFruits[i]), RequestedFruits[i]);
                    children.RemoveAt(j, 1);
            }
            int32 Index = 0;
            FruitType.Find(RequestedFruits[i], Index);
            AllFruits.Add(FruitPath[Index], 1);
        }
    }
    if (World) {
        World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &AFruitController::DestroyFallenFruits, 5.0f, false);
    }   
}

void AFruitController::CreateFruit(UStaticMeshComponent* Box, FString Path, FString Type) {
    UStaticMesh* CurrentFruitMesh = LoadObjFromPath<UStaticMesh>(FName(*Path));
    FString CurrentFruitType = Type;

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

void AFruitController::RandomFruitGeneration(int32 Num) {
    RandomFruitPath = "";
    RandomFruitType = "";

    int32 Rand = FMath::RandRange(0, FruitType.Num() - 1);
    CurrentClass = TypeAndClass.FindRef(FruitType[Rand]);

    if (Counter == 0) {
        PreviousClass = CurrentClass;
    }

    if (Counter == Num / 2) {
        if (PreviousClass == "Fruit")
            PreviousClass = "Vegetable";
        else {
            PreviousClass = "Fruit";
        }
    }

    if (PreviousClass != CurrentClass) {
        return RandomFruitGeneration(Num);
    }

    if (!AllFruits.Contains(FruitPath[Rand])) {
        AllFruits.Add(FruitPath[Rand], 1);

        RandomFruitPath = FruitPath[Rand];
        RandomFruitType = FruitType[Rand];

        if (Counter <= Num / 2) {
            PreviousClass = CurrentClass;
            Counter++;
        }
        return;
    }

    int32 count = AllFruits.FindRef(FruitPath[Rand]);
    if (count >= 2) {
        FruitPath.RemoveAt(Rand, 1);
        FruitType.RemoveAt(Rand, 1);
        return RandomFruitGeneration(Num);
    }

    AllFruits.Add(FruitPath[Rand], 2);

    RandomFruitPath = FruitPath[Rand];
    RandomFruitType = FruitType[Rand];

    if (Counter <= Num / 2) {
        PreviousClass = CurrentClass;
        Counter++;
    }
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

