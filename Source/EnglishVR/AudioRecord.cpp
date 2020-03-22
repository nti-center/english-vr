#include "AudioRecord.h"

UAudioRecord::UAudioRecord() {
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAudioRecord::BeginPlay() {
    Super::BeginPlay();
}


// Called every frame
void UAudioRecord::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

