// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

AMyCharacter::AMyCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
    Box->SetGenerateOverlapEvents(true);

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

void AMyCharacter::SetPath(TArray<AActor*> Path) {
    CurrentPath = Path;
}

void AMyCharacter::Go() {
    if (WalkingCount < CurrentPath.Num())
        if (GetController() && Cast<AAIController>(GetController()))
            Cast<AAIController>(GetController())->MoveToActor(CurrentPath[WalkingCount], -1.f, true, true);
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

void AMyCharacter::LoadRandomTexture(FString TextureName, FName ParametrName, FString path, int32 Rand, UMaterialInstanceDynamic* DynamicMaterial) {
    TextureName.Append(FString::FromInt(Rand));
    path = path + TextureName + "." + TextureName + "'";
    UTexture* RandomTexture = LoadObjFromPath<UTexture>(FName(*path));
    DynamicMaterial->SetTextureParameterValue(ParametrName, RandomTexture);
}

void AMyCharacter::SetRandomOutfit() {
    USkeletalMeshComponent* CharacterMesh;
    UMaterialInterface* Material;
    UMaterialInstanceDynamic* DynamicMaterial;

    CharacterMesh = GetMesh();
    if (!CharacterMesh) {
        UE_LOG(LogTemp, Warning, TEXT("Not found character mesh"));
        return;
    }

    FString Path = "";
    FString MaterialName = "";

    if (this->GetName() == "Girl1") {
        Path = "Material'/Game/Models/Girl_New/";
        MaterialName = "Girl_New_material.Girl_New_material'";
    }
    else if (this->GetName() == "Boy1") {
        return;
    }

    Material = LoadObjFromPath<UMaterialInterface>(FName(*(Path.Append(MaterialName))));

    if (!Material) {
        UE_LOG(LogTemp, Warning, TEXT("Can not take material"));
        return;
    }
       
    DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(CharacterMesh, Material);
    CharacterMesh->SetMaterial(0,DynamicMaterial);

    int32 Rand = FMath::RandRange(1,1);

    LoadRandomTexture("BaseColor", "BaseColor",Path, Rand, DynamicMaterial);
    LoadRandomTexture("ORM", "ORM", Path, Rand,  DynamicMaterial);
    LoadRandomTexture("Normal", "Normal", Path, Rand, DynamicMaterial);
}


void AMyCharacter::BeginPlay() {
    Super::BeginPlay();

    SetRandomOutfit();

    BubbleText = Cast<UBubleTextWidgetClass>(WidgetComponent->GetUserWidgetObject());

    for (TActorIterator<AVRPawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
       Pawn = Cast<AVRPawn>(*ActorItr);
        if (Pawn)
        {
            //UE_LOG(LogTemp, Warning, TEXT("Find Pawn"));
            return;
        }
    }


    WalkingCount = 0;
}

void AMyCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (!Pawn)
        return;

    FVector LocationVector = Pawn->GetActorLocation();
    //FVector UpVector = Pawn->GetActorUpVector();
    FVector WorldUp = FVector::UpVector;
    FRotator Rotator = BubbleText->MyLookRotation(GetActorLocation(),LocationVector, WorldUp);
    WidgetComponent->SetRelativeRotation(Rotator);
}

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

