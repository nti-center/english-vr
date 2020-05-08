// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeechRecognizerSphinx.h"

USpeechRecognizerSphinx::USpeechRecognizerSphinx() {
    PrimaryComponentTick.bCanEverTick = true;

    UE_LOG(LogTemp, Warning, TEXT("Start sphinx"));

    cmd_ln_t* config;

    config = cmd_ln_init(NULL, ps_args(), TRUE,
        "-hmm", *(FPaths::ProjectDir() + "/en-us/en-us"),
        "-lm", *(FPaths::ProjectDir() + "/en-us/en-us.lm.bin"),
        "-dict", *(FPaths::ProjectDir() + "/en-us/cmudict-en-us.dict"),
        NULL);

    if (config == NULL) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create config object, see log for details"));
        return;
    }

    ps = ps_init(config);
    if (ps == NULL) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create recognizer, see log for details"));
        return;
    }
}

void USpeechRecognizerSphinx::BeginPlay() {
    Super::BeginPlay();
}

void USpeechRecognizerSphinx::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpeechRecognizerSphinx::Recognize(const FString& File) {
    FILE* fh = fopen(TCHAR_TO_ANSI(*(File + ".wav")), "rb");
    if (fh == NULL) {
        UE_LOG(LogTemp, Warning, TEXT("Unable to open input file"));
        return;
    }

    int rv = ps_start_utt(ps);
    int16 buf[512];

    while (!feof(fh)) {
        size_t nsamp;
        nsamp = fread(buf, 2, 512, fh);
        rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }

    rv = ps_end_utt(ps);
    int32 score;
    char const* hyp = ps_get_hyp(ps, &score);

    fclose(fh);
    
    UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(hyp));
    OnRecognized.Broadcast(FString(hyp));
}

