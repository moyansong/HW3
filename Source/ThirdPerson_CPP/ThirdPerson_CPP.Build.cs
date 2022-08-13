// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ThirdPerson_CPP : ModuleRules
{
	public ThirdPerson_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
        //PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });



        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

        //PrivateDependencyModuleNames.AddRange(new string[] {  });

        // 如使用Slate UI，则取消注释
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // 如使用在线功能，则取消注释
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //      if (UEBuildConfiguration.bCompileSteamOSS == true)
        //      {
        //          DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //      }
        // }
    }
}
