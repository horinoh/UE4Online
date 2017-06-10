// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4OnlineTarget : TargetRules
{
	public UE4OnlineTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.Add("UE4Online");
		bUsesSteam = true;
	}
}
