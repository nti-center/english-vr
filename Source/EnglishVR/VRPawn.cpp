// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

// Sets default values
AVRPawn::AVRPawn() {
     // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    AudioCapture = CreateDefaultSubobject<UAudioCaptureComponent>(TEXT("AudioCapture"));
    AudioCapture->SetupAttachment(RootComponent);

    SoundSubmix = CreateDefaultSubobject<USoundSubmix>(TEXT("SoundSubmix"));
}

void AVRPawn::ResetPositionAndRotation() {
    FVector ActorLocation = GetActorLocation();
    FRotator ActorRotation = GetActorRotation();
    if (!ResetPoint) {
        return;
    }
    
    ActorLocation = ResetPoint->GetActorLocation();
    ActorRotation = ResetPoint->GetActorRotation();			
    
    SetActorLocation(ActorLocation);
    SetActorRotation(ActorRotation);
    
    //UE_LOG(LogTemp, Warning, TEXT("Actor Location  %s ActorRotation %s"), *ActorLocation.ToString(), *ActorRotation.ToString());

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay() {
    Super::BeginPlay();
}

// Called every frame
void AVRPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAction("ResetPlayerPosition", IE_Pressed, this, &AVRPawn::ResetPositionAndRotation);
}

