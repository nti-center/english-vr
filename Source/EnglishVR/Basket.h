// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "CollectableItem.h"
#include "Basket.generated.h"

UCLASS()
class ENGLISHVR_API ABasket : public AActor {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    USphereComponent* FillSphere;

    //UPROPERTY(BlueprintReadWrite)
    //TEnumAsByte<TreeType> Type;
    
    UPROPERTY(EditAnywhere)
    TMap<FString, int32> CountItems;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
    // Sets default values for this actor's properties
    ABasket();    

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
