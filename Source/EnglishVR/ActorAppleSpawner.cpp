// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorAppleSpawner.h"

// Sets default values
AActorAppleSpawner::AActorAppleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh"));
    SceneComponent->SetupAttachment(RootComponent);
    SceneComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

//void AActorAppleSpawner::OnActorSpawned() 
//{
//
//}

// Called when the game starts or when spawned
void AActorAppleSpawner::BeginPlay()
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
    
    FVector Origin(0, 0, 0);
    FVector BoxExtent(0, 0, 0);
    
    for (int i = 0; i < 100; i++) {   
        AActor *NewActor = World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, GetActorLocation() + FVector(BoxExtent.X * 2 * i, 0, 0), GetActorRotation(), SpawnParams);
        if (NewActor)
            NewActor->GetActorBounds(false, Origin, BoxExtent);
    }

    //UE_LOG(LogTemp, Warning, TEXT("Done"));
}

// Called every frame
void AActorAppleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

