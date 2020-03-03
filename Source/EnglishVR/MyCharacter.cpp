// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetGenerateOverlapEvents(true);
	Box->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(RootComponent);
	//Audio->Activate();

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

bool AMyCharacter::IsNotPlaying()
{
	if (Audio->IsPlaying())
		return false;
	else
		return true;
}

bool AMyCharacter::IsCorrectFruitsCount(TMap<FString, int32> _A, TMap<FString, int32> _B)
{
	TArray<FString> keys;

	_A.GenerateKeyArray(keys);
	
	for (int i = 0; i < keys.Num(); i++)
	{
		if (_B.Contains(keys[i]))
		{
			int32* a = _A.Find(keys[i]);
			int32* b = _B.Find(keys[i]);

			if (a != b)
			{
				return false;
			}

		}
		else
		{
			return false;
		}
		return true;
	}
	return false;
}

void AMyCharacter::GoToMarket()
{
	if(walkingCount < ToPath.Num())
		ai->MoveToActor(ToPath[walkingCount],-1.f,true,true);
}

void AMyCharacter::GoAway()
{
	walkingCount = 0;

	if (!isTmp && isEnd)
	{
		if (walkingCount < OutPath.Num())
		{
			ai->MoveToActor(OutPath[walkingCount], -1.f, true, true);
		}

		isTmp = false;
	}
}

void AMyCharacter::GetABasket()
{
	if ((EPickupState == EStatesEnum::Finished) && !isEnd)
	{
		//Basket = Cast<ABasket>(Basket);
		Basket->Mesh->SetSimulatePhysics(false);
		//Attach Insaide
		Basket->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Basket->Mesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHandSocket");
		PlayDialog("goodbye3");
		isEnd = true;
	}
}


void AMyCharacter::PlayDialog(FName DialogName)
{
	FString ContextString;
	USoundCue* cue;

	if (isCheck == true)
	{
		FAudioDataTableStruct* Row = DataTable->FindRow<FAudioDataTableStruct>(DialogName, ContextString, true);
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

				Audio->SetSound(cue);
				Audio->Play();
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

	if (!Audio)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant find Audio"));
		//return;
	}

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
	GoToMarket();

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if (IsNotPlaying())
	//{
	//	if (EComeState == EStatesEnum::Active)
	//	{
	//		PlayDialog("greetings4", DataTable, Audio, isCheck);
	//		//FOnAudioFinished OnAudioFinished;
	//	
	//		PlayDialog("requests4", DataTable, Audio, isCheck);
	//		EComeState = EStatesEnum::Finished;
	//	}

	// GoAway();
	//}

}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
		return;

	if (OtherActor && OtherActor != this)
	{
		if (IsNotPlaying())
		{
			if (!(EPickupState == EStatesEnum::Finished))
			{
				if (IsCorrectFruitsCount(FruitsCount, Basket->CountItems))
				{
					EPickupState = EStatesEnum::Active;
				}
				else
				{
					PlayDialog("errors3");
					ENegativeState = EStatesEnum::Active;
				}
			}
		}
	}
}

void AMyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
		return;

	if (OtherActor && OtherActor != this)
	{
		if (IsNotPlaying())
		{
			if (!(EPickupState == EStatesEnum::Finished))
			{
				EPickupState = EStatesEnum::NotActive;
			}
		}
	}

}


// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

