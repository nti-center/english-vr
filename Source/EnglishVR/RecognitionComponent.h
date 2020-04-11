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
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseReceivedDelegate, FString, Response);

public:
    //DECLARE_DYNAMIC_MULTICAST_DELEGATE(FYourDelegate, FChangedEvent)
    //FChangedEvent& OnChanged() { return ChangedEvent; }
    //
    //UPROPERTY(BlueprintAssignable)
    //FYourDelegate delegateVariableName;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FResponseReceivedDelegate OnResponseReceived;

    UFUNCTION(BlueprintCallable)
    void Recognize(const FString &FilePath);

    void ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    // Sets default values for this actor's properties
    URecognitionComponent();

private:
    FHttpModule* Http;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
