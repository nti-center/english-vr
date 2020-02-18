// Fill out your copyright notice in the Description page of Project Settings.


#include "Basket.h"

// Sets default values
ABasket::ABasket() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    //Root->SetupAttachment(RootComponent);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelPath(TEXT("/Game/Models/Environment/Basket/BasketMesh.BasketMesh"));

    if (!ModelPath.Succeeded())
        return;

    Mesh->SetStaticMesh(ModelPath.Object);
    Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    //Mesh->bEditableWhenInherited = true;

    FillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FillSphere"));    
    FillSphere->SetGenerateOverlapEvents(true);
    FillSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasket::OnOverlapBegin);
    FillSphere->OnComponentEndOverlap.AddDynamic(this, &ABasket::OnOverlapEnd);
    FillSphere->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    //FillSphere->bEditableWhenInherited = true;
    //FillSphere->SetSphereRadius(12.0f);
    //FillSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 11.0f));
}

void ABasket::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    //UE_LOG(LogTemp, Warning, TEXT("2 wrong"));
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;

    ICollectable* Item = Cast<ICollectable>(OtherActor);
    if (!OtherActor->GetClass()->ImplementsInterface(UCollectable::StaticClass()))
        return;

    //OtherActor->RootComponent->AttachToComponent(RootComponent, Attachmentrule
    //OtherActor->SetupAttachment(RootComponent);
    FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
    OtherActor->AttachToActor(this, AttachmentTransformRules);
    //OtherActor->GetRootComponent()->AttachToComponent(FillSphere, AttachmentTransformRules);
    //FillSphere->SetupAttachment()
    UE_LOG(LogTemp, Warning, TEXT("Attached %d"), Mesh->GetAttachChildren().Num());

    FString Type = ICollectable::Execute_GetType(OtherActor);
    
    //Actor->SetRootComponent(Component);
    //Actor->AddInstanceComponent(Component);

    if (!CountItems.Contains(Type))
        CountItems.Add(Type, 0);
    ++CountItems[Type];
    //UE_LOG(LogTemp, Warning, TEXT("2 wrong"));
    //OtherActor->Destroy();    
}

void ABasket::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;

    ICollectable* Item = Cast<ICollectable>(OtherActor);
    if (!OtherActor->GetClass()->ImplementsInterface(UCollectable::StaticClass()))
        return;

    //OtherActor->AttachToComponent(FillSphere, FAttachmentTransformRules::KeepWorldTransform);

    FString Type = ICollectable::Execute_GetType(OtherActor);

    if (CountItems.Contains(Type))
        --CountItems[Type];
}

// Called when the game starts or when spawned
void ABasket::BeginPlay() {
    Super::BeginPlay();

}

// Called every frame
void ABasket::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

}

