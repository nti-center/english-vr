// Fill out your copyright notice in the Description page of Project Settings.


#include "FilledCrate.h"

// Sets default values
AFilledCrate::AFilledCrate()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);    

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelPath(TEXT("/Game/Models/Crate/CrateMesh.CrateMesh"));
    
    if (ModelPath.Succeeded()) {
        Mesh->SetStaticMesh(ModelPath.Object);
        Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        Mesh->SetRelativeScale3D(FVector(0.03f, 0.02f, 0.01f)); 
    }
    
    //AppleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AppleMesh"));    
        //UE_LOG(LogTemp, Warning, TEXT("Extent %s"), *AppleMesh->GetStaticMesh()->GetBounds().GetBox().GetSize().ToString());
    
}

// Called when the game starts or when spawned
void AFilledCrate::BeginPlay()
{
    Super::BeginPlay();

    if (!FruitMesh) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find Mesh"));
        return;
    }

    if (!FruitBP) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find Blueprint"));
        return;
    }
    
    UBlueprint* GeneratedBP = FruitBP;
    
    UWorld* World = GetWorld();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    FVector CrateOrigin(0, 0, 0);
    FVector CrateBoxExtent(0, 0, 0);
    GetActorBounds(false, CrateOrigin, CrateBoxExtent);
    CrateOrigin -= CrateBoxExtent;

    FVector FruitBoxExtent = FruitMesh->GetBounds().GetBox().GetSize();
    FVector MaxSize = (CrateBoxExtent - BorderSize) * 2;
    MaxSize.Z += BorderSize.Z + FruitBoxExtent.Z;

    for (int i = 0;; i++) {
        float CurrX = FruitBoxExtent.X / 2.0F + FruitBoxExtent.X * i;
        if (CurrX > MaxSize.X) break;           

        for (int j = 0;; j++) {
            float CurrY = FruitBoxExtent.Y / 2.0f + FruitBoxExtent.Y * j;
            if (CurrY > MaxSize.Y) break;

            for (int k = 0;; k++) {
                float CurrZ = FruitBoxExtent.Z / 2.0f + FruitBoxExtent.Z * k;
                if (CurrZ > MaxSize.Z) break;

                World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, CrateOrigin + BorderSize + FVector(CurrX, CurrY, CurrZ), GetActorRotation(), SpawnParams);
            }
        }
    }
}

// Called every frame
void AFilledCrate::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);  
}

