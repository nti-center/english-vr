// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(RootComponent);

	PlayerMesh = GetMesh();
	if (PlayerMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Mesh loaded"));
		PlayerMesh->SetSkeletalMesh(AlternateMeshAsset);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObject(TEXT("DataTable'/Game/CSV/MainDialog.MainDialog'"));
	if (DataTableObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
		DataTable = DataTableObject.Object;
		isCheck = true;
	}
}

void AMyCharacter::GoToMarket(TArray<ATargetPoint*> pathArray)
{
	//AActor* ActorTarget;

	for (int32 i = pathArray.Num(); i > 0; i--)
	{
		UE_LOG(LogTemp, Warning, TEXT("i = %d"), i);
		//pathArray[i] = Cast<ATargetPoint>(ActorTarget);
		//MoveToActor();
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//for (TActorIterator<MotionControllerPawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	_areaLightingController = Cast<MotionControllerPawn>(*ActorItr);
	//	if (_areaLightingController)
	//	{
	//		LOG("APrimaryPlayerController: GetAreaLightingController(): controller found!");
	//		return;
	//	}
	//}

	GoToMarket(ToPath);
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

