/* Copyright (C) 2018 Ivan Garcia, Juan Jose Corroto and Javier Cordoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

using UnrealBuildTool;

public class Alliance : ModuleRules
{
	public Alliance(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "Networking", "Sockets", "OnlineSubsystem", "OnlineSubsystemSteam",
             "OnlineSubsystemUtils" });

        PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "DlgSystem" });
    }
}
