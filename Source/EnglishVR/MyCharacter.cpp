// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

AMyCharacter::AMyCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
    Box->SetGenerateOverlapEvents(true);
    //Box->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
    //Box->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd);

    PhrasesAudio = CreateDefaultSubobject<UPhrasesAudioComponent>(TEXT("PhrasesAudio"));
    PhrasesAudio->SetupAttachment(RootComponent);

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);

    AnimationState = EAnimationState::None;
}

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path) {
    if (Path == NAME_None) return NULL;
    return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

bool AMyCharacter::IsCorrectFruitsCount() {
    if (FruitsCount.Num() != Basket->FruitCounts.Num())
        return false;

    for (auto& Item : FruitsCount)
        if (!Basket->FruitCounts.Contains(Item.Key) || Item.Value != Basket->FruitCounts[Item.Key])
            return false;

    return true;
}

void AMyCharacter::GoToMarket() {
	if (WalkingCount < ToPath.Num()) {
        if (GetController() && Cast<AAIController>(GetController())) {
            Cast<AAIController>(GetController())->MoveToActor(ToPath[WalkingCount], -1.f, true, true);
        }
	}
}

void AMyCharacter::SetPath(TArray<AActor*> Path) {
    CurrentPath = Path;
}

void AMyCharacter::GoAway() {

    if (WalkingCount < OutPath.Num())
        if (GetController() && Cast<AAIController>(GetController()))
            Cast<AAIController>(GetController())->MoveToActor(OutPath[WalkingCount], -1.f, true, true);

    IsTmp = false;
}

void AMyCharacter::Go() {
    if (WalkingCount < CurrentPath.Num())
        if (GetController() && Cast<AAIController>(GetController()))
            Cast<AAIController>(GetController())->MoveToActor(CurrentPath[WalkingCount], -1.f, true, true);
}

void AMyCharacter::TakeBasket() {

    if (Basket == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Can't take basket: basket is null"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("TakeBasket"));

    Basket->Mesh->SetSimulatePhysics(false);
    Basket->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Basket->AttachOverlappingActors();
    FAttachmentTransformRules Atr(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
    Basket->AttachToComponent(GetMesh(), Atr, "RightHandSocket");
   // PlayDialog(DialogList.FindRef("gratitude"), IsCheck);
    
   // TArray<FName> tmp;
   // tmp.Add(DialogList.FindRef("goodbye"));
   // PlayRequestList(tmp, 1 ,IsCheck);

    IsEnd = true;
}

bool AMyCharacter::TakeBasket(ABasket* NewBasket) {
    if (NewBasket == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Can't take basket: basket is null"));
        return false;
    }
    UE_LOG(LogTemp, Warning, TEXT("TakeBasket"));

    NewBasket->Mesh->SetSimulatePhysics(false);
    NewBasket->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    NewBasket->AttachOverlappingActors();
    FAttachmentTransformRules Atr(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
    NewBasket->AttachToComponent(GetMesh(), Atr, "RightHandSocket");

    return true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay() {
    Super::BeginPlay();

    IsCheck = true;
    WalkingCount = 0;

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)  {
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;
    if (/*!IsNotPlaying() ||*/ EPickupState == EStatesEnum::Finished || !Cast<ABasket>(OtherActor))
        return;

    int32 count = 0;

    Basket = Cast<ABasket>(OtherActor);

    for (auto& Elem : AllRequestsFruitsAndCountList){
        UE_LOG(LogTemp, Warning, TEXT("Key %s value %d"), *Elem.Key, Elem.Value);
        for (auto& Elem_2 : Basket->AllFruitCounts){

            if ((Elem.Key == Elem_2.Key) && (Elem.Value == Elem_2.Value)) {
                count++;
            }
            UE_LOG(LogTemp, Warning, TEXT("Basket Key %s Basket value %d"), *Elem.Key, Elem.Value);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Count %d Num %d"), count, AllRequestsFruitsAndCountList.Num());

   if (count == AllRequestsFruitsAndCountList.Num()) {
       EPickupState = EStatesEnum::Active;   
   }
   else {
       ENegativeState = EStatesEnum::Active;
   }
}

void AMyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
        return;

    if (/*!IsNotPlaying() ||*/ EPickupState == EStatesEnum::Finished || !Cast<ABasket>(OtherActor))
        return;

    EPickupState = EStatesEnum::NotActive;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::AddFruitRequest(FString Type, int32 Count) {
    if (FruitsCount.Contains(Type))
        FruitsCount[Type] += Count;
    else if (Count > 0)
        FruitsCount.Emplace(Type, Count);
}

void AMyCharacter::ClearFruitRequests() {
    FruitsCount.Empty();
}
