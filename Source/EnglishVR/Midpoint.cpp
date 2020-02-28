// Fill out your copyright notice in the Description page of Project Settings.


#include "Midpoint.h"


AMidpoint::AMidpoint()
{
	//Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//Sphere->SetupAttachment(RootComponent);


	FillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FillSphere"));
	FillSphere->SetGenerateOverlapEvents(true);
	FillSphere->OnComponentBeginOverlap.AddDynamic(this, &AMidpoint::OnOverlapBegin);
	FillSphere->OnComponentEndOverlap.AddDynamic(this, &AMidpoint::OnOverlapEnd);
}

void AMidpoint::BeginPlay()
{
	//UE_LOG(LogTemp, Warning, TEXT("Begin play"));
	//if (!Sphere)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not sphere"));
	//	return;
	//}

	//Sphere->SetGenerateOverlapEvents(true);
	//Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMidpoint::OnOverlapBegin);
}

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

void AMidpoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
