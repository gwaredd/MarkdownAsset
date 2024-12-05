// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

#pragma once

#include "Interfaces/IPluginManager.h"
#include "Brushes/SlateImageBrush.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)

class FMarkdownAssetEditorStyle : public FSlateStyleSet
{
public:

	FMarkdownAssetEditorStyle() : FSlateStyleSet( "MarkdownAssetEditorStyle" )
	{
		const FVector2D Icon16x16( 16.0f, 16.0f );
		const FVector2D Icon64x64( 64.0f, 64.0f );

		SetContentRoot( IPluginManager::Get().FindPlugin( "MarkdownAsset" )->GetContentDir() );
		Set( "ClassIcon.MarkdownAsset", new IMAGE_BRUSH( "markdown_16", Icon16x16 ) );
		Set( "ClassThumbnail.MarkdownAsset", new IMAGE_BRUSH( "markdown_64", Icon64x64 ) );

		FSlateStyleRegistry::RegisterSlateStyle( *this );
	}

	~FMarkdownAssetEditorStyle()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle( *this );
	}
};


#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
