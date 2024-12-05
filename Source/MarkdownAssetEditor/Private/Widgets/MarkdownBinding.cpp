// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

#include "MarkdownBinding.h"
#include "HelperFunctions/MarkdownAssetEditorStatics.h"

void UMarkdownBinding::OpenURL( FString URL )
{
    FPlatformProcess::LaunchURL( *URL, nullptr, nullptr );
}

void UMarkdownBinding::OpenAsset( FString URL )
{
	MarkdownAssetStatics::TryToOpenAsset(URL);
}
