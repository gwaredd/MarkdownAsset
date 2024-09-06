#include "MarkdownBinding.h"

void UMarkdownBinding::OpenURL( FString URL )
{
    FPlatformProcess::LaunchURL( *URL, nullptr, nullptr );
}

void UMarkdownBinding::OpenAsset( FString URL )
{
    GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(URL);
}
