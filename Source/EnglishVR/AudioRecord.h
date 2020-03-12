// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AudioRecord.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENGLISHVR_API UAudioRecord : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAudioRecord();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRecord = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString  PathToRecord;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
