#pragma once

#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"
#include "UObject/ObjectMacros.h"
#include "MarkdownAssetEditorSettings.generated.h"


UCLASS( config = Editor )
class MARKDOWNASSETEDITOR_API UMarkdownAssetEditorSettings : public UObject
{
	GENERATED_BODY()

	public:

		UPROPERTY( config, EditAnywhere, Category = Appearance )
		bool bDarkSkin;

	public:

		UMarkdownAssetEditorSettings();
};
