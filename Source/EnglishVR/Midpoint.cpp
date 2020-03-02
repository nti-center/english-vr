// Fill out your copyright notice in the Description page of Project Settings.


#include "Midpoint.h"


AMidpoint::AMidpoint()
{
	FillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FillSphere"));
	FillSphere->SetupAttachment(RootComponent);
	FillSphere->SetGenerateOverlapEvents(true);
	FillSphere->OnComponentBeginOverlap.AddDynamic(this, &AMidpoint::OnOverlapBegin);
	FillSphere->OnComponentEndOverlap.AddDynamic(this, &AMidpoint::OnOverlapEnd);
}

void AMidpoint::BeginPlay()
{
	
}

void AMidpoint::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
		return;

	//UE_LOG(LogTemp, Warning, TEXT("Trigger"));

	if (OtherActor && OtherActor != this)
	{
		//GLog->Log(TEXT("TriggerOverlaped"));

		Character = Cast<AMyCharacter>(OtherActor);
		if (Character)
		{
			Character->walkingCount++;
			Character->GoToMarket();
		}
		
	}

}

void AMidpoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
