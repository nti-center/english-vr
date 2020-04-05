// Fill out your copyright notice in the Description page of Project Settings.


#include "FruitBox.h"

// Sets default values for this component's properties
UFruitBox::UFruitBox() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    //Box->AttachToComponent(this, );
    Box->SetupAttachment(this);
    //this->Bo
    //static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelPath(TEXT("/Game/Models/Crate/CrateMesh.CrateMesh"));
    //
    //if (!ModelPath.Succeeded())
    //    return;


    //Mesh->SetStaticMesh(ModelPath.Object);
    Box->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Box->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));


    //AppleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AppleMesh"));    
    //UE_LOG(LogTemp, Warning, TEXT("Extent %s"), *AppleMesh->GetStaticMesh()->GetBounds().GetBox().GetSize().ToString());
}

// Called when the game starts
void UFruitBox::BeginPlay() {
    Super::BeginPlay();

    if (!FruitMesh) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find Mesh"));
        return;
    }

    if (!FruitBP) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find Blueprint"));
        return;
    }

    //Cast<AActorWithMesh>(FruitBP)->Mesh->SetStaticMesh(FruitMesh);

    UBlueprint* GeneratedBP = FruitBP;
    //GeneratedBP->Get
    UWorld* World = GetWorld();
    FActorSpawnParameters SpawnParams;    
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    FVector FruitBoxBE = Box->GetScaledBoxExtent();
    FVector FruitBoxOrigin = Box->GetComponentLocation() - FruitBoxBE;

    FVector FruitBE = FruitMesh->GetBounds().BoxExtent;
    FVector FruitOffset = FruitBE / 3.0f;
    
    FVector MaxSize = FruitBoxBE * 2;
    FIntVector Count(MaxSize / (FruitBE * 2));
    FVector OriginOffset = FruitBoxBE - FVector(Count) * FruitBE;

    //UE_LOG(LogTemp, Warning, TEXT("Extent %s"), *Count.ToString());

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
                float OffsetZ = FMath::RandRange(-FruitOffset.Z, FruitOffset.Z);
                float CurrZ = FruitBE.Z + FruitBE.Z * 2 * k;
                if (CurrZ + FruitBE.Z + OffsetZ > MaxSize.Z)
                    break;
                
                FRotator RRotator(FMath::RandRange(-40, 40), FMath::RandRange(-40, 40), FMath::RandRange(-40, 40));
                FVector FruitLocation = FVector(CurrX + ROffsetX, CurrY + ROffsetY, CurrZ + OffsetZ);
                FVector Location = Box->GetComponentLocation() + OriginOffset + Box->GetComponentRotation().RotateVector(FruitLocation - FruitBoxBE);
                World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, Location, RRotator, SpawnParams);
            }
        }
    }
}

// Called every frame
void UFruitBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}