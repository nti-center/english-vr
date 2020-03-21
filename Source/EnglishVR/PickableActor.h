// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UObject/Interface.h"
#include "PickableActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickableActor : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class ENGLISHVR_API IPickableActor
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PickableActor")
    void Pickup(class USceneComponent *AttachTo);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PickableActor")
    void Drop();
};
