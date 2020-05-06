// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeechRecognizer.h"

USpeechRecognizer::USpeechRecognizer() {
    PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void USpeechRecognizer::BeginPlay() {
    Super::BeginPlay();
}


// Called every frame
void USpeechRecognizer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

