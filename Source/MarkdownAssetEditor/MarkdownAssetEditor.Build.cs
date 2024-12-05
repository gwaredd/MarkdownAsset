// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

using UnrealBuildTool;

public class MarkdownAssetEditor : ModuleRules
{
	public MarkdownAssetEditor( ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        DynamicallyLoadedModuleNames.AddRange( new string[] {
            "AssetTools",
            "MainFrame",
        });

        PrivateIncludePaths.AddRange( new string[] {
            "MarkdownAssetEditor/Private",
            "MarkdownAssetEditor/Private/Factories",
            "MarkdownAssetEditor/Private/Shared",
            "MarkdownAssetEditor/Private/Styles",
            "MarkdownAssetEditor/Private/Toolkits",
            "MarkdownAssetEditor/Private/Widgets",
        });

        PrivateDependencyModuleNames.AddRange( new string[] {
            "ContentBrowser",
            "Core",
            "CoreUObject",
            "DesktopWidgets",
            "EditorStyle",
            "Engine",
            "InputCore",
            "Projects",
            "Slate",
            "SlateCore",
            "UnrealEd",
			"MarkdownAsset",
            "WebBrowser",
            "DeveloperSettings",
            "ToolMenus",
            "AssetDefinition",
        });

        PrivateIncludePathModuleNames.AddRange( new string[] {
            "AssetTools",
            "UnrealEd",
        });
    }
}
