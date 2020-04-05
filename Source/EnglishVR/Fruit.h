// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor.h"
#include "Collectable.h"
#include "Fruit.generated.h"

UCLASS()
class ENGLISHVR_API AFruit : public AActor, public IPickableActor, public ICollectable {
    GENERATED_BODY()

public:
    AFruit();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMeshComponent* Mesh;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

};
