// Fill out your copyright notice in the Description page of Project Settings.


#include "MControllerClass.h"

// Sets default values
AMControllerClass::AMControllerClass() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMControllerClass::BeginPlay() {
    Super::BeginPlay();
}

// Called every frame
void AMControllerClass::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AMControllerClass::GetActorHearHand1(AActor *& NearestMesh) {
    NearestMesh = nullptr;
}

