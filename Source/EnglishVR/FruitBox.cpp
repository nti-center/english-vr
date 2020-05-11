// Fill out your copyright notice in the Description page of Project Settings.


#include "FruitBox.h"

UFruitBox::UFruitBox() {
    PrimaryComponentTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(this);

    Box->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Box->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

    //static ConstructorHelpers::FClassFinder<AFruit> PickableBPClass(TEXT("/Game/Blueprints/PickableBP"));
    //if (!PickableBPClass.Succeeded()) {
    //    UE_LOG(LogTemp, Warning, TEXT("Cant find PickableBPClass"));
    //    return;
    //}
    
    FruitClass = AFruit::StaticClass();
}

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path) {
    if (Path == NAME_None) return NULL;
    return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

void UFruitBox::BeginPlay() {
    Super::BeginPlay();

    if (!FruitMesh || !FruitClass) {
        if (!Controller) {
            UE_LOG(LogTemp, Warning, TEXT("Cant find controller"));
            return;
        }

        TArray<FString> tmp = Controller->RandomFruitGeneration();
        FruitMesh = LoadObjFromPath<UStaticMesh>(FName(*tmp[0]));
        FruitType = tmp[1];
    }

    UWorld* World = GetWorld();
    FActorSpawnParameters SpawnParams;    
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    FVector FruitBoxBE = Box->GetScaledBoxExtent().GetAbs();
    FVector FruitBoxOrigin = Box->GetComponentLocation() - FruitBoxBE;

    //UE_LOG(LogTemp, Error, TEXT("Old Location is: %s"), *Box->GetComponentLocation().ToString());

    FVector FruitBE = FruitMesh->GetBounds().BoxExtent;
    FVector FruitOffset = FruitBE / 2.0f;
    
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

               // FRotator RRotator(FMath::RandRange(-40, 40), FMath::RandRange(-40, 40), FMath::RandRange(-40, 40));
                FRotator RRotator(0, 0, 0);
                if (FruitType == "Carrot" || FruitType == "Cucumber") {
                    RRotator = FRotator(70, 0, 0);
                }
                FVector FruitLocation = FVector(CurrX + ROffsetX, CurrY + ROffsetY, CurrZ + ROffsetZ);
                FVector Location = Box->GetComponentLocation() + OriginOffset + Box->GetComponentRotation().RotateVector(FruitLocation - FruitBoxBE);

                //UE_LOG(LogTemp, Error, TEXT("Fruit"));

                AFruit* Fruit = World->SpawnActor<AFruit>(FruitClass, Location, RRotator, SpawnParams);

                if (Fruit) {
                    Fruit->Mesh->SetStaticMesh(FruitMesh);
                    Fruit->Type = FruitType;
                }
            }
        }
    }
}

void UFruitBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}