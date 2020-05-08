// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EnglishVR : ModuleRules {
    public EnglishVR(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "Http", "Json", "JsonUtilities" });
    
        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        bEnableExceptions = true;

        RuntimeDependencies.Add("$(BinaryOutputDir)/pocketsphinx.dll", "$(ProjectDir)/SpeechRecognition/Sphinx/pocketsphinx/lib/pocketsphinx.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/sphinxbase.dll", "$(ProjectDir)/SpeechRecognition/Sphinx/sphinxbase/lib/sphinxbase.dll");
        PublicAdditionalLibraries.Add("$(ProjectDir)/SpeechRecognition/Sphinx/pocketsphinx/lib/pocketsphinx.lib");
        PublicAdditionalLibraries.Add("$(ProjectDir)/SpeechRecognition/Sphinx/sphinxbase/lib/sphinxbase.lib");
        PublicIncludePaths.Add("../SpeechRecognition/Sphinx/pocketsphinx/include");
        PublicIncludePaths.Add("../SpeechRecognition/Sphinx/sphinxbase/include");
        PublicIncludePaths.Add("../SpeechRecognition/Sphinx/sphinxbase/include/win32");

        RuntimeDependencies.Add("$(BinaryOutputDir)/Microsoft.CognitiveServices.Speech.core.dll", "$(ProjectDir)/packages/Microsoft.CognitiveServices.Speech.1.11.0/runtimes/win-x64/native/Microsoft.CognitiveServices.Speech.core.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/Microsoft.CognitiveServices.Speech.extension.codec.dll", "$(ProjectDir)/packages/Microsoft.CognitiveServices.Speech.1.11.0/runtimes/win-x64/native/Microsoft.CognitiveServices.Speech.extension.codec.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/Microsoft.CognitiveServices.Speech.extension.kws.dll", "$(ProjectDir)/packages/Microsoft.CognitiveServices.Speech.1.11.0/runtimes/win-x64/native/Microsoft.CognitiveServices.Speech.extension.kws.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/Microsoft.CognitiveServices.Speech.extension.silk_codec.dll", "$(ProjectDir)/packages/Microsoft.CognitiveServices.Speech.1.11.0/runtimes/win-x64/native/Microsoft.CognitiveServices.Speech.extension.silk_codec.dll");
        PublicAdditionalLibraries.Add("$(ProjectDir)/packages/Microsoft.CognitiveServices.Speech.1.11.0/build/native/x64/Release/Microsoft.CognitiveServices.Speech.core.lib");
        PublicIncludePaths.Add("../packages/Microsoft.CognitiveServices.Speech.1.11.0/build/native/include/cxx_api");
        PublicIncludePaths.Add("../packages/Microsoft.CognitiveServices.Speech.1.11.0/build/native/include/c_api");
    }
}
