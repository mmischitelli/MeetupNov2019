// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MeetupNov2019 : ModuleRules
{
	public MeetupNov2019(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.NoSharedPCHs;
        PrivatePCHHeaderFile = "MeetupNov2019.h";
        CppStandard = CppStandardVersion.Cpp17;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
