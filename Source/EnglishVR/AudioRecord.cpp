// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioRecord.h"

// Sets default values for this component's properties
UAudioRecord::UAudioRecord(){
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAudioRecord::BeginPlay(){
    Super::BeginPlay();
}


// Called every frame
void UAudioRecord::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    //UE_LOG(LogTemp, Warning, TEXT("Is Record %s"), (IsRecord ? TEXT("True") : TEXT("False")));
}
