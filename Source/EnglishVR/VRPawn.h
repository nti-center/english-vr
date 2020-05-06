// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

UCLASS()
class ENGLISHVR_API AVRPawn : public APawn {
    GENERATED_BODY()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAudioRecordedDelegate, FString, File);

public:
    // Sets default values for this pawn's properties
    AVRPawn();
    
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    ATargetPoint* ResetPoint;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FAudioRecordedDelegate OnAudioRecorded;
    
    void ResetPositionAndRotation();

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
	// Called every frame
    virtual void Tick(float DeltaTime) override;
    
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
