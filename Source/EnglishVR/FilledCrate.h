// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "FilledCrate.generated.h"

UCLASS()
class ENGLISHVR_API AFilledCrate : public AActor
{
	GENERATED_BODY()

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* CrateMesh;

public:	
	// Sets default values for this actor's properties
	AFilledCrate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
