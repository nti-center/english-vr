// Fill out your copyright notice in the Description page of Project Settings.


#include "Basket.h"

// Sets default values
ABasket::ABasket() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelPath(TEXT("/Game/Models/Basket/BasketMesh.BasketMesh"));

    if (ModelPath.Succeeded()) {
        Mesh->SetStaticMesh(ModelPath.Object);
        Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    }

    FillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FillSphere"));    
    FillSphere->SetGenerateOverlapEvents(true);
    FillSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasket::OnOverlapBegin);
    FillSphere->OnComponentEndOverlap.AddDynamic(this, &ABasket::OnOverlapEnd);
    FillSphere->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    FillSphere->SetSphereRadius(12.0f);
    FillSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 11.0f));
}

void ABasket::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;

    ACollectableItem* Item = Cast<ACollectableItem>(OtherActor);
    
    if (!Item)
        return;

    if (!CountItems.Contains(Item->Type))
        CountItems.Add(Item->Type, 0);
    ++CountItems[Item->Type];
    //UE_LOG(LogTemp, Warning, TEXT("2 wrong"));
    //OtherActor->Destroy();    
}

void ABasket::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;

    ACollectableItem* Item = Cast<ACollectableItem>(OtherActor);

    if (!Item)
        return;

    if (CountItems.Contains(Item->Type))
        --CountItems[Item->Type];
}

// Called when the game starts or when spawned
void ABasket::BeginPlay() {
    Super::BeginPlay();

}

// Called every frame
void ABasket::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

}

