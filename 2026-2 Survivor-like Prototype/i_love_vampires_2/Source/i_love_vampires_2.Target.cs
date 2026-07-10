// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class i_love_vampires_2Target : TargetRules
{
	public i_love_vampires_2Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "i_love_vampires_2" } );

        //BuildEnvironment = TargetBuildEnvironment.Unique;
        bOverrideBuildEnvironment = true;
        bUseLoggingInShipping = true;
    }
}
