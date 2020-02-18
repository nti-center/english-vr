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
    
    UWorld* World = GetWorld();
    //FActorSpawnParameters SpawnParams;
    //SpawnParams.Owner = GetOwner();
    //SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    FVector CrateBoxExtent = Box->Bounds.BoxExtent;
    FVector CrateOrigin = Box->Bounds.Origin - CrateBoxExtent;
    
    FVector FruitBoxExtent = FruitMesh->GetBounds().BoxExtent;
    
    FVector MaxSize = CrateBoxExtent * 2;

    UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Box->GetRelativeRotation().ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Box->GetOwner()->GetActorRotation().ToString());
    ////UE_LOG(LogTemp, Warning, TEXT("%s"), *Box->rota);
    UE_LOG(LogTemp, Warning, TEXT("-----------------"));
    //Box->Rotat
    for (int i = 0;; i++) {
        float CurrX = FruitBoxExtent.X + FruitBoxExtent.X * 2 * i;
        if (CurrX > MaxSize.X) break;
    
        for (int j = 0;; j++) {
            float CurrY = FruitBoxExtent.Y + FruitBoxExtent.Y * 2 * j;
            if (CurrY > MaxSize.Y) break;
    
            for (int k = 0;; k++) {
                float CurrZ = FruitBoxExtent.Z + FruitBoxExtent.Z * 2 * k;
                if (CurrZ > MaxSize.Z) break;
                
                
                World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, CrateOrigin + FVector(CurrX, CurrY, CurrZ), Box->GetRelativeRotation());
            }
        }
    }
}

// Called every frame
void UFruitBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}