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

	//ai = Cast<AAIController>(thisCharacter->GetController());
}

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
{
	if (Path == NAME_None) return NULL;
	return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

void AMyCharacter::GoToMarket(TArray<AActor*> pathArray, AAIController* _ai)
{
	for (int32 i = pathArray.Num(); i > 0; i--)
	{
		//UE_LOG(LogTemp, Warning, TEXT("i = %d"), i);
		_ai->MoveToActor(pathArray[i]);
	}
}

void AMyCharacter::PlayDialog(FName DialogName, UDataTable* table, UAudioComponent* _audio, bool check)
{
	FString ContextString;
	USoundCue* cue;

	if (check == true)
	{
		FAudioDataTableStruct* Row = table->FindRow<FAudioDataTableStruct>(DialogName, ContextString, true);
		if (Row)
		{
			FString output = (*Row).Path;
			GLog->Log(output);

			FName path = FName(*output);

			if (path != NAME_None)
			{
				cue = LoadObjFromPath<USoundCue>(path);
				_audio->SetSound(cue);
				_audio->Play();
			}
			else
			{
				GLog->Log("Path is empty!");
			}
		}
	}
}

bool AMyCharacter::IsState(UEnum * A, UEnum * B)
{
	if (A == B)
		return true;
	else
		return false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//	GoToMarket(ToPath, ai);
	//  PlayDialog("greetings4",DataTable,Audio,isCheck);

	//if (!ai)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not found ai"));
	//}

	//for (TActorIterator<MotionControllerPawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	_areaLightingController = Cast<MotionControllerPawn>(*ActorItr);
	//	if (_areaLightingController)
	//	{
	//		LOG("APrimaryPlayerController: GetAreaLightingController(): controller found!");
	//		return;
	//	}
	//}


	
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

