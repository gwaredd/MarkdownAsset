// Copyright (C) 2024 El Burro Games - All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MarkdownAsset.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "MarkdownAssetDeveloperSettings.generated.h"

/**
 * 
 */
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

	const FString& GetRelativeDocumentationFolderPath() const
	{
		return DocumentationDirectory.Path;
	}

	const FString& GetDefaultPrefix() const
	{
		return DefaultPrefix;
	}

	const bool ShouldRemovePrefix() const
	{
		return bTryToRemoveAssetsPrefixesOnDocumentationCreation;
	}

protected:

	virtual FName GetCategoryName() const override { return FName(TEXT("Markdown")); }
	
	virtual FName GetSectionName() const override { return FName(TEXT("Markdown Documentation Settings")); }

	/** The file that will be opened from the "Open Documentation" button in the LevelEditor toolbar. */
	UPROPERTY(Config, EditDefaultsOnly)
	TSoftObjectPtr<UMarkdownAsset> DocumentationMainFile;

	/**  */
	UPROPERTY(Config, EditDefaultsOnly)
	TMap<TSoftObjectPtr<UObject>, TSoftObjectPtr<UMarkdownAsset>> DocumentationFilesPerObject;

	/** All documentation files will be created in this directory by default. */
	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation)
	FDirectoryPath DocumentationDirectory = FDirectoryPath("/Game/Documentation");

	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation)
	FString DefaultPrefix = FString(TEXT("MD_"));

	/** If true, when creating a new associated MarkdownAsset from it, the prefixs will try to be removed. */
	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation)
	bool bTryToRemoveAssetsPrefixesOnDocumentationCreation = true;
};
