// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class XunFeiSpeechToText : ModuleRules
{
	public XunFeiSpeechToText(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
				"Json"
			}
			);

		string thirdPartyPath = Path.Combine(ModuleDirectory, "../../ThirdParty");
		string xunFeiIATPath = Path.Combine(thirdPartyPath, "XunFeiIAT");


		PublicIncludePaths.Add(Path.Combine(xunFeiIATPath,"include"));
		
		if (Target.Platform == UnrealTargetPlatform.Win32)
		{
			PublicAdditionalLibraries.Add(Path.Combine(xunFeiIATPath,"libs","msc.lib"));
			// RuntimeDependencies.Add(Path.Combine(xunFeiIATPath,"bin","msc.dll"));
			
			// 仅仅添加这一行,可以把DLL文件拷贝的插件的Binary目录和项目的binary目录,可以正常使用
			RuntimeDependencies.Add( "$(BinaryOutputDir)/msc.dll",Path.Combine(xunFeiIATPath,"bin","msc.dll"));
		}

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(xunFeiIATPath,"libs","msc_x64.lib"));
			// RuntimeDependencies.Add(Path.Combine(xunFeiIATPath,"bin","msc_x64.dll"));
			
			// 仅仅添加这一行,可以把DLL文件拷贝的插件的Binary目录和项目的binary目录,可以正常使用
			RuntimeDependencies.Add( "$(BinaryOutputDir)/msc_x64.dll",Path.Combine(xunFeiIATPath,"bin","msc_x64.dll"));
		}
	}
}
