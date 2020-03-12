// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "Engine.h"
#include <string>
//#include "JsonUtilities.h"
//#include <format>
#include "RecognitionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENGLISHVR_API URecognitionComponent : public USceneComponent {
    GENERATED_BODY()

public:
    //UPROPERTY()
    FHttpModule* Http;

    /* The actual HTTP call */
    UFUNCTION(BlueprintCallable)
    void Recognize(const FString &FilePath);

    /*Assign this function to call when the GET request processes sucessfully*/
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    // Sets default values for this actor's properties
    URecognitionComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
