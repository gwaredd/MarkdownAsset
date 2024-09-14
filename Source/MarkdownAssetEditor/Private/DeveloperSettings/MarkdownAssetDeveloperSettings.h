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

protected:

	virtual FName GetCategoryName() const override { return FName(TEXT("Markdown")); }
	
	virtual FName GetSectionName() const override { return FName(TEXT("Markdown Documentation Settings")); }

	/** The file that will be opened from the "Open Documentation" button in the LevelEditor toolbar. */
	UPROPERTY(Config, EditDefaultsOnly)
	TSoftObjectPtr<UMarkdownAsset> DocumentationMainFile;
};
