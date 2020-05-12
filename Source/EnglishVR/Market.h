// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Market.generated.h"

UCLASS()
class ENGLISHVR_API AMarket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMarket();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UWidgetComponent* WidgetComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
