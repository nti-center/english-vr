
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
    FillSphere->SetupAttachment(Mesh);
    
    OutlineSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FillBox"));
    OutlineSphere->SetGenerateOverlapEvents(true);
    OutlineSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasket::OnOutlineOverlapBegin);
    OutlineSphere->OnComponentEndOverlap.AddDynamic(this, &ABasket::OnOutlineOverlapEnd);
    OutlineSphere->SetupAttachment(Mesh);
    //FillSphere->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    //FillSphere->bEditableWhenInherited = true;
    //FillSphere->SetSphereRadius(12.0f);
    //FillSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 11.0f));
}

void ABasket::AttachOverlappingActors() {
    TArray<AActor*> Actors;
    FillSphere->GetOverlappingActors(Actors);
    
    for (auto& Actor : Actors) {
        if (UKismetSystemLibrary::DoesImplementInterface(Actor, UCollectable::StaticClass())) {
            FAttachmentTransformRules Atr(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
            Actor->AttachToComponent(Mesh, Atr);
        
            UActorComponent* ActorMesh = Actor->GetComponentByClass(UStaticMeshComponent::StaticClass());
            if (ActorMesh != nullptr)
        	    Cast<UStaticMeshComponent>(ActorMesh)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}

void ABasket::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    //UE_LOG(LogTemp, Warning, TEXT("2 wrong"));
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;
    
    ICollectable* Item = Cast<ICollectable>(OtherActor);
    if (!OtherActor->GetClass()->ImplementsInterface(UCollectable::StaticClass()))
        return;
    
    FString Type = ICollectable::Execute_GetType(OtherActor);
    
    //Actor->SetRootComponent(Component);
    //Actor->AddInstanceComponent(Component);
    
    if (!FruitCounts.Contains(Type))
        FruitCounts.Add(Type, 0);
    ++FruitCounts[Type];
    //UE_LOG(LogTemp, Warning, TEXT("2 wrong"));
    //OtherActor->Destroy();    
}

void ABasket::OnOutlineOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
    	return;
    
    AMControllerClass *MController = Cast<AMControllerClass>(OtherActor);
    if (MController) {
        Mesh->SetRenderCustomDepth(true);
        Mesh->SetCustomDepthStencilValue(2);
    }
}

void ABasket::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;
    
    ICollectable* Item = Cast<ICollectable>(OtherActor);
    if (!OtherActor->GetClass()->ImplementsInterface(UCollectable::StaticClass()))
        return;
    
    //OtherActor->AttachToComponent(FillSphere, FAttachmentTransformRules::KeepWorldTransform);
    
    FString Type = ICollectable::Execute_GetType(OtherActor);

    if (FruitCounts.Num() != 0) {
        if (FruitCounts.Contains(Type))
            --FruitCounts[Type];
        if (FruitCounts[Type] == 0) {
            FruitCounts.Remove(Type);
        }
    }
}

void ABasket::OnOutlineOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;
    
    AMControllerClass *MController = Cast<AMControllerClass>(OtherActor);
    if (MController) {
        Mesh->SetRenderCustomDepth(false);
        Mesh->SetCustomDepthStencilValue(0);
    }
}

// Called when the game starts or when spawned
void ABasket::BeginPlay() {
    Super::BeginPlay();
    
    Mesh->SetSimulatePhysics(true);
    Mesh->SetMassOverrideInKg("", 3, true);
}

// Called every frame
void ABasket::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

