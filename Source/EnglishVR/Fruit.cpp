// Fill out your copyright notice in the Description page of Project Settings.


#include "Fruit.h"

AFruit::AFruit() {
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

void AFruit::BeginPlay() {
    Super::BeginPlay();

    if (!Mesh) {
        UE_LOG(LogTemp, Warning, TEXT("Cant find Mesh"));
        return;
    }

    Mesh->SetSimulatePhysics(true);

}

void AFruit::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

}

