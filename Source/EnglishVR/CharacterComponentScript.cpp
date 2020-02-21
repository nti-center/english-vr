// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponentScript.h"

// Sets default values for this component's properties
UCharacterComponentScript::UCharacterComponentScript()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(this);

	Box->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Box->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}


// Called when the game starts
void UCharacterComponentScript::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterComponentScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

