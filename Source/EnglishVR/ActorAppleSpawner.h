// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "ActorAppleSpawner.generated.h"

UCLASS()
class ENGLISHVR_API AActorAppleSpawner : public AActor
{
	GENERATED_BODY()
        
    UPROPERTY(VisibleAnywhere)
    USceneComponent* SceneComponent;

public:    
	// Sets default values for this actor's properties
	AActorAppleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
