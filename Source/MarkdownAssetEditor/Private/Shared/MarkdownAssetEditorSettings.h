// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

#pragma once

#include "MarkdownAsset.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"
#include "UObject/ObjectMacros.h"
#include "MarkdownAssetEditorSettings.generated.h"

//TODO (Maxi): Check if there is a modern way to do this.
UCLASS(config=Editor)
class MARKDOWNASSETEDITOR_API UMarkdownAssetEditorSettings : public UObject
{
	GENERATED_BODY()

public:
	UMarkdownAssetEditorSettings();

	bool ShouldRemovePrefix() const
	{
		return bTryToRemoveAssetsPrefixesOnDocumentationCreation;
	}

	bool ShouldOpenNewFiles() const
	{
		return bAutoOpenNewlyCreatedFiles;
	}

	//NOTE (Maxi): Keeping this public so I don't mess with the current code using this directly. Might be refactored later.
	UPROPERTY( config, EditAnywhere, Category = Appearance )
	bool bDarkSkin;

protected:
	
	/** If true, when creating a new associated MarkdownAsset from it, the prefix will try to be removed. */
	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation)
	bool bTryToRemoveAssetsPrefixesOnDocumentationCreation = true;

	/** If true, when a MarkdownAsset file is created, the content browser will open it automatically. */
	UPROPERTY(Config, EditDefaultsOnly, Category=AssetCreation)
	bool bAutoOpenNewlyCreatedFiles = true;

	/** EXPERIMENTAL: If opening files becomes too slow, turning this on should make everything smoother.
	 * Untested. */
	UPROPERTY(Config, EditDefaultsOnly, Category=Memory, AdvancedDisplay)
	bool bShouldCacheMarkdownFiles = false;
};
