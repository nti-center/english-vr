// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "ActorAppleSpawner.generated.h"

UCLASS()
class ENGLISHVR_API AActorAppleSpawner : public AActor {
    GENERATED_BODY()
        
    UPROPERTY(VisibleAnywhere)
    USceneComponent* SceneComponent;

public:    
    AActorAppleSpawner();

protected:
    virtual void BeginPlay() override;

public:	
    virtual void Tick(float DeltaTime) override;

};
