// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
//#include "ActorWithMesh.h"
#include "FruitBox.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENGLISHVR_API UFruitBox : public USceneComponent {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Crate")
    UBoxComponent* Box;
    
    //UPROPERTY(EditAnywhere, Category = "Crate")
    //FVector BorderSize;

    UPROPERTY(EditAnywhere, Category = "Fruit")
    UStaticMesh* FruitMesh;

    UPROPERTY(EditAnywhere, Category = "Fruit")
    UBlueprint* FruitBP;

public:
    // Sets default values for this component's properties
    UFruitBox();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
