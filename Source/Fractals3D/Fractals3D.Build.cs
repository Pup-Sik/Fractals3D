// Copyright PupSik, 2023. All Rights Reserved.

using UnrealBuildTool;

public class Fractals3D : ModuleRules
{
	public Fractals3D(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

    #if UE_5_00_OR_LATER
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Projects",
                "RenderCore",
                "ToolWidgets",
                "Slate",
                "Json",
                "JsonUtilities",
                "SlateCore",
                "InputCore",
                "EditorFramework",
                "EditorStyle",
                "UnrealEd",
                "LevelEditor",
                "InteractiveToolsFramework",
                "EditorInteractiveToolsFramework"
				// ... add private dependencies that you statically link with here ...	
			}
            );

    #elif UE_4_17_OR_LATER
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Engine",
                "Projects",
                "RenderCore",
                "Slate",
                "Json",
                "JsonUtilities",
                "SlateCore",
                "InputCore",
                "EditorStyle",
                "UnrealEd",
                "LevelEditor",
                "UnrealEd",
            }
			);
    #endif
    }
}
