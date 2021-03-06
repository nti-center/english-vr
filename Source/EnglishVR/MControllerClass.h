// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MControllerClass.generated.h"

UCLASS()
class ENGLISHVR_API AMControllerClass : public AActor {
    GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    AMControllerClass();

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "MotionColntroller")
    void GetActorHearHand1(AActor *& NearestMesh);
};
