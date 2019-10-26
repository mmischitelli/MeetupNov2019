// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class MeetupNov2019EditorTarget : TargetRules
{
	public MeetupNov2019EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "MeetupNov2019" } );
	}
}
