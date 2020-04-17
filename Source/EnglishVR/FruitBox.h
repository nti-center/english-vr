// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "FruitBoxController.h"
#include "Fruit.h"
#include "FruitBox.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENGLISHVR_API UFruitBox : public USceneComponent {
    GENERATED_BODY()

public:
    UFruitBox();

    UPROPERTY(EditAnywhere, Category = "Crate")
    UBoxComponent* Box;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fruit")
    UStaticMesh* FruitMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fruit")
    FString FruitType;

    UPROPERTY(EditAnywhere, Category = "Fruit")
    AFruitBoxController* Controller;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fruit")
    TSubclassOf<class AFruit> FruitClass;

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
