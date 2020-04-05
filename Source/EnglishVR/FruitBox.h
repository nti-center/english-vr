// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "FruitBox.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENGLISHVR_API UFruitBox : public USceneComponent {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Crate")
    UBoxComponent* Box;

    UPROPERTY(EditAnywhere, Category = "Fruit")
    UStaticMesh* FruitMesh;

    UPROPERTY(EditAnywhere, Category = "Fruit")
    UBlueprint* FruitBP;

public:
    UFruitBox();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
