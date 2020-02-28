// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObject(TEXT("DataTable'/Game/CSV/DataTable.DataTable'"));
	if (DataTableObject.Succeeded())
	{	
		DataTable = DataTableObject.Object;
		isCheck = true;

		UE_LOG(LogTemp, Warning, TEXT("Data table loaded"));
	}
}

template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
{
	if (Path == NAME_None) return NULL;
	return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

bool AMyCharacter::IsState(EStatesEnum A, EStatesEnum B)
{
	if (A == B)
		return true;
	else
		return false;
}

bool AMyCharacter::IsNotPlaying(UAudioComponent * _audio)
{
	if (_audio->IsPlaying())
		return false;
	else
		return true;
}

void AMyCharacter::GoToMarket(TArray<AActor*> _toPath, AAIController* _ai, int32 _walkingCount)
{
	for (int32 i = 0; i < _toPath.Num(); i++)
	{
		_ai->MoveToActor(_toPath[0],-1.f,true,true);
	}
}

void AMyCharacter::PlayDialog(FName DialogName, UDataTable* _dataTable, UAudioComponent* _audio, bool _isCheck)
{
	FString ContextString;
	USoundCue* cue;

	if (_isCheck == true)
	{
		FAudioDataTableStruct* Row = _dataTable->FindRow<FAudioDataTableStruct>(DialogName, ContextString, true);
		if (Row)
		{
			
			FString output = (*Row).Path;
			GLog->Log(output);

			FName path = FName(*output);

			if (path != NAME_None)
			{
				UE_LOG(LogTemp, Warning, TEXT("Path"));

				cue = LoadObjFromPath<USoundCue>(path);
				float duration = cue->GetDuration();

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


// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	walkingCount = 0;

	//FVector FruitBoxBE = Box->Bounds.BoxExtent;
	//UE_LOG(LogTemp, Warning, TEXT("BoxEtent is %s"),*FruitBoxBE.ToString());
	//if (!Audio)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Cant find Audio"));
	//	return;
	//}

	PlayerMesh = GetMesh();
	if (PlayerMesh)
	{
		PlayerMesh->SetSkeletalMesh(AlternateMeshAsset);
		UE_LOG(LogTemp, Warning, TEXT("Player Mesh loaded"));
	}

	ai = Cast<AAIController>(thisCharacter->GetController());

	if (!ai)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not found ai"));
		return;
	}

	//ai->MoveToActor(ToPath[0]);
	GoToMarket(ToPath, ai, walkingCount);

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//if (IsNotPlaying(Audio))
	//{
	//	//if (EComeState == EStatesEnum::Active)
	//	//{
	//	//	PlayDialog("greetings4", DataTable, Audio, isCheck);
	//	//	//FOnAudioFinished OnAudioFinished;
	//	//
	//	//	PlayDialog("requests4", DataTable, Audio, isCheck);
	//	//	EComeState = EStatesEnum::Finished;
	//	//}
	//	UE_LOG(LogTemp, Warning, TEXT("Audio not playing"));
	//}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

