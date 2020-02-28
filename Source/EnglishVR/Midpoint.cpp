// Fill out your copyright notice in the Description page of Project Settings.


#include "Midpoint.h"


void AMidpoint::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Trigger"));

	if (OtherActor && OtherActor != this)
	{
		GLog->Log(TEXT("TriggerOverlaped"));

		Character = Cast<AMyCharacter>(OtherActor);
		if (Character)
		{
			Character->walkingCount++;
			UE_LOG(LogTemp, Warning, TEXT("Walking count is %d"), Character->walkingCount);
		}
		
	}

}

AMidpoint::AMidpoint()
{

}

void AMidpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}