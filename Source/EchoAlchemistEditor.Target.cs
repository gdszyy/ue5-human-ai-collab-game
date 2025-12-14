// Copyright 2025 Voidzyy. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EchoAlchemistEditorTarget : TargetRules
{
	public EchoAlchemistEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		ExtraModuleNames.AddRange( new string[] { "EchoAlchemist" } );
	}
}
