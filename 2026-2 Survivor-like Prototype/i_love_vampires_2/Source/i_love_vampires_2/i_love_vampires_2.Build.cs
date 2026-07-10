// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class i_love_vampires_2 : ModuleRules
{
	public i_love_vampires_2(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;                // disable Unity builds

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "EnhancedInput", "GameplayTags", "UMG", "Slate", "SlateCore"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		bUseRTTI = true;

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
