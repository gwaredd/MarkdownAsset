#include "MarkdownBinding.h"

void UMarkdownBinding::OpenURL( FString URL )
{
    FPlatformProcess::LaunchURL( *URL, nullptr, nullptr );
}
