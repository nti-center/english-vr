// Fill out your copyright notice in the Description page of Project Settings.


#include "FilledCrate.h"

// Sets default values
AFilledCrate::AFilledCrate()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CrateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    CrateMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelPath(TEXT("/Game/Models/Crate/CrateMesh.CrateMesh"));

    if (ModelPath.Succeeded()) {
        CrateMesh->SetStaticMesh(ModelPath.Object);
        CrateMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        CrateMesh->SetRelativeScale3D(FVector(0.03f, 0.02f, 0.01f)); 
    }

    UE_LOG(LogTemp, Warning, TEXT("Privet"));
}

// Called when the game starts or when spawned
void AFilledCrate::BeginPlay()
{
    Super::BeginPlay();

    UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, 
          TEXT("/Game/Blueprints/Apple.Apple")));

    UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
    if (!SpawnActor) {
        UE_LOG(LogTemp, Warning, TEXT("CANT FIND OBJECT TO SPAWN"));
        return;
    }

    UClass* SpawnClass = SpawnActor->StaticClass();
    if (SpawnClass == NULL) {
        UE_LOG(LogTemp, Warning, TEXT("CLASS == NULL"));
        return;
    }

    UWorld* World = GetWorld();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FVector CrateOrigin(0, 0, 0);
    FVector CrateBoxExtent(0, 0, 0);    
    GetActorBounds(false, CrateOrigin, CrateBoxExtent);
    FVector CrateOffset = CrateBoxExtent * 0.1;

    CrateOrigin -= CrateBoxExtent;    

    FVector ActorOrigin(0, 0, 0);
    FVector ActorBoxExtent(0, 0, 0);

    FVector MaxSize = CrateBoxExtent * 2 - CrateOffset * 2;

    UE_LOG(LogTemp, Warning, TEXT("Max %s"), *MaxSize.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Offset %s"), *CrateOffset.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Extent %s"), *CrateBoxExtent.ToString());

    for (int i = 0;; i++) {
        float CurrX = 5 + ActorBoxExtent.X * 2 * i;
        if (CurrX > MaxSize.X) break;

        for (int j = 0;; j++) {
            float CurrY = 20 + ActorBoxExtent.Y * 2 * j;
            if (CurrY > MaxSize.Y) break;

            for (int k = 0; k < 1; k++) {
                float CurrZ = 20 + ActorBoxExtent.Z * 2 * k;
                //if (CurrZ > MaxSize.Z) break;

                AActor* NewActor = World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, CrateOrigin + CrateOffset + FVector(CurrX, CurrY, CurrZ),
                    GetActorRotation(), SpawnParams);
                NewActor->GetActorBounds(false, ActorOrigin, ActorBoxExtent);
            }
        }
    }

    // UE_LOG(LogTemp, Warning, TEXT("Done"));
}

// Called every frame
void AFilledCrate::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);  
}

