// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Collectable.h"
#include "MControllerClass.h"
#include "PickableActor.h"
#include "Basket.generated.h"

UCLASS(Abstract)
class ENGLISHVR_API ABasket : public AActor, public IPickableActor {
    GENERATED_BODY()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFruitAddedDelegate);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFruitRemovedDelegate);

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void OnOutlineOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOutlineOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    ABasket();
    
    UFUNCTION()
    void AttachOverlappingActors();
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMeshComponent* Mesh;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USphereComponent* FillSphere;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USphereComponent* OutlineSphere;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<FString, int32> FruitCounts;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<FString, int32> AllFruitCounts;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FFruitAddedDelegate OnFruitAdded;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FFruitRemovedDelegate OnFruitRemoved;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
