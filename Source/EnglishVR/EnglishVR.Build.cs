// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EnglishVR : ModuleRules {
    public EnglishVR(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "Http", "Json", "JsonUtilities",
            "AudioCapture", "AudioMixer", "Networking", "Sockets" });
    
        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        RuntimeDependencies.Add("$(BinaryOutputDir)/portaudio_x64.dll", "$(ProjectDir)/SpeechRecognition/DeepSpeech/Portaudio/lib/portaudio_x64.dll");
        PublicAdditionalLibraries.Add("$(ProjectDir)/SpeechRecognition/DeepSpeech/Portaudio/lib/portaudio_x64.lib");
        PublicIncludePaths.Add("../SpeechRecognition/DeepSpeech/Portaudio/include");

        RuntimeDependencies.Add("$(BinaryOutputDir)/Microsoft.CognitiveServices.Speech.core.dll", "$(ProjectDir)/SpeechRecognition/MicrosoftCognitiveServicesSpeech/lib/Microsoft.CognitiveServices.Speech.core.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/Microsoft.CognitiveServices.Speech.extension.codec.dll", "$(ProjectDir)/SpeechRecognition/MicrosoftCognitiveServicesSpeech/lib/Microsoft.CognitiveServices.Speech.extension.codec.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/Microsoft.CognitiveServices.Speech.extension.kws.dll", "$(ProjectDir)/SpeechRecognition/MicrosoftCognitiveServicesSpeech/lib/Microsoft.CognitiveServices.Speech.extension.kws.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/Microsoft.CognitiveServices.Speech.extension.silk_codec.dll", "$(ProjectDir)/SpeechRecognition/MicrosoftCognitiveServicesSpeech/lib/Microsoft.CognitiveServices.Speech.extension.silk_codec.dll");
        PublicAdditionalLibraries.Add("$(ProjectDir)/SpeechRecognition/MicrosoftCognitiveServicesSpeech/lib/Microsoft.CognitiveServices.Speech.core.lib");
        PublicIncludePaths.Add("../SpeechRecognition/MicrosoftCognitiveServicesSpeech/include/cxx_api");
        PublicIncludePaths.Add("../SpeechRecognition/MicrosoftCognitiveServicesSpeech/include/c_api");
    }
}
