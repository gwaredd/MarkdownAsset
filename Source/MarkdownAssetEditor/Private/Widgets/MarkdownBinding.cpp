#include "MarkdownBinding.h"

#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

void UMarkdownBinding::OpenURL( FString URL )
{
    FPlatformProcess::LaunchURL( *URL, nullptr, nullptr );
}

void UMarkdownBinding::OpenAsset( FString URL )
{
	const FSoftObjectPath AssetPath(URL);
	
	UObject* Object = FindObject<UObject>(AssetPath.GetAssetPath());
	if (!Object)
	{
		Object = LoadObject<UObject>(nullptr, *AssetPath.GetAssetPathString(), nullptr, LOAD_NoRedirects);
	}

	if (Object)
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Object);
	}
	else
	{
	    const FString InfoContent = TEXT("Warning: Asset Path not valid!");
	    FNotificationInfo* Info = new FNotificationInfo(FText::FromString(InfoContent));
			
	    Info->bUseLargeFont = true;
	    Info->ExpireDuration = 5.0f;
	    Info->WidthOverride = 500.0f;
	    Info->bUseSuccessFailIcons = true;
		Info->Image = FAppStyle::GetBrush(TEXT("Icons.Warning"));
		
	    FSlateNotificationManager::Get().AddNotification(*Info);
	}
	
}
