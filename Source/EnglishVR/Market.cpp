// Fill out your copyright notice in the Description page of Project Settings.


#include "Market.h"

// Sets default values
AMarket::AMarket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AMarket::BeginPlay(){
	Super::BeginPlay();

    TArray <USceneComponent*> ChildrenComponents;
    this->GetComponents<USceneComponent>(ChildrenComponents);
    
    if(ChildrenComponents.Num() > 0){
        for (int i = 0; i < ChildrenComponents.Num(); i++) {
            if (ChildrenComponents[i]->GetName() == "FruitCostWidget") {
                WidgetComponent = Cast<UWidgetComponent>(ChildrenComponents[i]);
                if(WidgetComponent)
                    UE_LOG(LogTemp, Error, TEXT("Find"));
            }
        }
    }
}

// Called every frame
void AMarket::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

}

