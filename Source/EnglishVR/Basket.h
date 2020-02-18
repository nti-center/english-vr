// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Collectable.h"
#include "Basket.generated.h"

UCLASS(Abstract)
class ENGLISHVR_API ABasket : public AActor {
    GENERATED_BODY()    

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
    // Sets default values for this actor's properties
    ABasket();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USphereComponent* FillSphere;

    UPROPERTY(EditAnywhere)
    TMap<FString, int32> CountItems;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
