// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DebandPlugin : ModuleRules
{
	public DebandPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"Core",
				"Engine",
				"RenderCore",
				"Renderer",
				"RHI",
				"Projects"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"Engine", 
				"RenderCore", 
				"Renderer", 
				"RHI"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"Renderer",
				"RenderCore",
				"RHI",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"RHI",
				"Renderer",
				"Projects",
				"RenderCore",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
