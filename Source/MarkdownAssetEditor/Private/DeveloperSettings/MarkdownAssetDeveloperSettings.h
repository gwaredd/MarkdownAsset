// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MarkdownAsset.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "MarkdownAssetDeveloperSettings.generated.h"

UCLASS(Config=DocumentationSettings, DefaultConfig)
class MARKDOWNASSETEDITOR_API UMarkdownAssetDeveloperSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:

	static const UMarkdownAssetDeveloperSettings* Get();

#if WITH_EDITOR
	void OpenEditorSettingWindow() const;
#endif //WITH_EDITOR

	const FSoftObjectPath& GetDocumentationMainFileSoftPath() const { return DocumentationMainFile.ToSoftObjectPath(); }

	void GetRelativeDocumentationFolderPath(FString& OutPath) const
	{
		OutPath = bUseDefaultFolder ? DocumentationDirectory.Path : TEXT("");
	}

	const FString& GetDefaultPrefix() const
	{
		return DefaultPrefix;
	}

	const FSoftObjectPath* GetMarkdownForAsset(const UObject* Asset) const
	{
		return MarkdownFilesPerAssets.Find(Asset);
	}

	void AddMarkdownAssetForFile(const FSoftObjectPath& Asset, const FSoftObjectPath& MarkdownAsset)
	{
		MarkdownFilesPerAssets.Add(Asset, MarkdownAsset);
	}

protected:

	virtual FName GetCategoryName() const override { return FName(TEXT("Markdown")); }
	
	virtual FName GetSectionName() const override { return FName(TEXT("Markdown Documentation Settings")); }

	// The file that will be opened from the "Open Documentation" button in the LevelEditor toolbar.
	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation )
	TSoftObjectPtr<UMarkdownAsset> DocumentationMainFile;

	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation )
	TMap<FSoftObjectPath, FSoftObjectPath> MarkdownFilesPerAssets;

	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation, meta=(InlineEditConditionToggle))
	bool bUseDefaultFolder = true;
	
	// If enabled and valid, when creating a new MarkdownAsset file from an Asset editor window or context action this
	// folder will be the default value in the asset creation Window.
	// If disabled or not valid, the same folder of the asset will be used instead.

	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation, meta=(EditCondition=bUseDefaultFolder))
	FDirectoryPath DocumentationDirectory = FDirectoryPath("/Game/Documentation");

	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation)
	FString DefaultPrefix = FString(TEXT("MD_"));

};
