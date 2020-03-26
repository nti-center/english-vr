// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "Engine/TargetPoint.h"
#include "Midpoint.generated.h"


UCLASS(Abstract)
class ENGLISHVR_API AMidpoint : public ATargetPoint {
    GENERATED_BODY()
    
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    AMidpoint();
    
    UPROPERTY(BlueprintReadWrite)
    AMyCharacter* Character;
    
    UPROPERTY(BlueprintReadWrite)
    USphereComponent* FillSphere;

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;
	
};
