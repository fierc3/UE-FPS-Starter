// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PSFPSCoreTarget : TargetRules
{
	public PSFPSCoreTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("PSFPSCore");

        // Allow overriding the build environment
        bOverrideBuildEnvironment = true;

        // Add your custom setting here, for example:
        GlobalDefinitions.Add("bStrictConformanceMode=0");
    }
}
