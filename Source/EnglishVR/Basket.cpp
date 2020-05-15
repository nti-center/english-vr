
#include "Basket.h"
#include "AudioDataTableStruct.h"

// Sets default values
ABasket::ABasket() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelPath(TEXT("/Game/Models/Environment/Basket/BasketMesh.BasketMesh"));
    
    if (!ModelPath.Succeeded())
    	return;
    
    Mesh->SetStaticMesh(ModelPath.Object);
    Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    
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

    static ConstructorHelpers::FObjectFinder<UDataTable> _DataTable(TEXT("DataTable'/Game/CSV/Fruits.Fruits'"));
    if (_DataTable.Succeeded()) {
        DataTable = _DataTable.Object;
    }
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
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;
    
    ICollectable* Item = Cast<ICollectable>(OtherActor);
    if (!OtherActor->GetClass()->ImplementsInterface(UCollectable::StaticClass()))
        return;
    
    FString Type = ICollectable::Execute_GetType(OtherActor);
    
    if (!FruitCounts.Contains(Type))
        FruitCounts.Add(Type, 0); 
    ++FruitCounts[Type];

    FAudioDataTableStruct* Row = DataTable->FindRow<FAudioDataTableStruct>(FName(*Type), ContextString, true);
    if (Row) {
        TotalCost += Row->Cost;
    }
    FString text = FString::FromInt(TotalCost);
    text.Append("$");
    CostWidget->SetText(text);

    OnFruitAdded.Broadcast();
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
    
    FString Type = ICollectable::Execute_GetType(OtherActor);
    if (FruitCounts.Contains(Type)) {
        --FruitCounts[Type];

        FAudioDataTableStruct* Row = DataTable->FindRow<FAudioDataTableStruct>(FName(*Type), ContextString, true);
        if (Row) {
            TotalCost -= Row->Cost;
        }
        FString text = FString::FromInt(TotalCost);
        text.Append("$");
        CostWidget->SetText(text);
    }
    if (FruitCounts.Contains(Type) && (FruitCounts[Type] == 0)) {
            FruitCounts.Remove(Type);
    }

    OnFruitRemoved.Broadcast();
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

    for (TActorIterator<AMarket> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
        Market = Cast<AMarket>(*ActorItr);
    }
    if (!Market)
        return;
    CostWidget = Cast<UFruitCostWidget>(Market->WidgetComponent->GetUserWidgetObject());
    if (!CostWidget)
        return; 
}

// Called every frame
void ABasket::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

