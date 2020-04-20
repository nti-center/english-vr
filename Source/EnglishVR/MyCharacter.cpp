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

void AMyCharacter::BeginPlay() {
    Super::BeginPlay();

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

    FVector UpVector = Pawn->GetActorUpVector();
    FVector LocationVector = Pawn->GetActorLocation();
    FRotator Rotator = MyLookRotation(LocationVector, UpVector);
    WidgetComponent->SetRelativeRotation(Rotator);
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

FRotator AMyCharacter::MyLookRotation(FVector lookAt, FVector upDirection) {
    FVector forward = lookAt - GetActorLocation();
    FVector up = upDirection;


    forward = forward.GetSafeNormal();
    up = up - (forward * FVector::DotProduct(up, forward));
    up = up.GetSafeNormal();

    FVector vector = forward.GetSafeNormal();
    FVector vector2 = FVector::CrossProduct(up, vector);
    FVector vector3 = FVector::CrossProduct(vector, vector2);
    float m00 = vector.X;
    float m01 = vector.Y;
    float m02 = vector.Z;
    float m10 = vector2.X;
    float m11 = vector2.Y;
    float m12 = vector2.Z;
    float m20 = vector3.X;
    float m21 = vector3.Y;
    float m22 = vector3.Z;

    float num8 = (m00 + m11) + m22;
    FQuat quaternion = FQuat();

    if (num8 > 0.0f)
    {
        float num = (float)FMath::Sqrt(num8 + 1.0f);
        quaternion.W = num * 0.5f;
        num = 0.5f / num;
        quaternion.X = (m12 - m21) * num;
        quaternion.Y = (m20 - m02) * num;
        quaternion.Z = (m01 - m10) * num;
        return FRotator(quaternion);
    }

    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = (float)FMath::Sqrt(((1.0f + m00) - m11) - m22);
        float num4 = 0.5f / num7;
        quaternion.X = 0.5f * num7;
        quaternion.Y = (m01 + m10) * num4;
        quaternion.Z = (m02 + m20) * num4;
        quaternion.W = (m12 - m21) * num4;
        return FRotator(quaternion);
    }

    if (m11 > m22)
    {
        float num6 = (float)FMath::Sqrt(((1.0f + m11) - m00) - m22);
        float num3 = 0.5f / num6;
        quaternion.X = (m10 + m01) * num3;
        quaternion.Y = 0.5f * num6;
        quaternion.Z = (m21 + m12) * num3;
        quaternion.W = (m20 - m02) * num3;
        return FRotator(quaternion);
    }

    float num5 = (float)FMath::Sqrt(((1.0f + m22) - m00) - m11);
    float num2 = 0.5f / num5;
    quaternion.X = (m20 + m02) * num2;
    quaternion.Y = (m21 + m12) * num2;
    quaternion.Z = 0.5f * num5;
    quaternion.W = (m01 - m10) * num2;

    return FRotator(quaternion);
}
