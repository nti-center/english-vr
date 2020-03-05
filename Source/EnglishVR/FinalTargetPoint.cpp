// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalTargetPoint.h"

AFinalTargetPoint::AFinalTargetPoint()
{
	FillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FillSphere"));
	FillSphere->SetupAttachment(RootComponent);
	FillSphere->SetGenerateOverlapEvents(true);
	FillSphere->OnComponentBeginOverlap.AddDynamic(this, &AFinalTargetPoint::OnOverlapBegin);
	FillSphere->OnComponentEndOverlap.AddDynamic(this, &AFinalTargetPoint::OnOverlapEnd);
}

void AFinalTargetPoint::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Trigger"));

	if (OtherActor && OtherActor != this)
	{
		Character = Cast<AMyCharacter>(OtherActor);
		if (Character)
		{
			Character->EComeState = EStatesEnum::Active;
            Character->WalkingCount = 0;
		}
	}
}

void AFinalTargetPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void AFinalTargetPoint::BeginPlay()
{
	
}
