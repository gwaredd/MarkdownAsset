#pragma once
#include "DeveloperSettings/MarkdownAssetDeveloperSettings.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FMarkdownAssetEditorStaticFunctions"

namespace MarkdownAssetStatics
{
	struct FHyperlinkData
	{
		FSimpleDelegate Hyperlink;
		FText HyperlinkText;

		bool IsValid() const
		{
			return Hyperlink.GetHandle().IsValid();
		}
	};
	
	static void TryToOpenAsset(const FSoftObjectPath& ObjectPath, const FText& MessageIfNotFound = FText::FromString(""),
		const FHyperlinkData& HyperlinkData = FHyperlinkData() )
	{
		UObject* Object = FindObject<UObject>(ObjectPath.GetAssetPath());
		if (!Object)
		{
			Object = LoadObject<UObject>(nullptr, *ObjectPath.GetAssetPathString(), nullptr, LOAD_NoRedirects);
		}

		if (Object)
		{
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Object);
		}
		else
		{
			//const FString InfoContent = TEXT("Warning: Asset Path not valid!");
			const FText TextFormat = LOCTEXT("MarkdownAsset_AssetNotFoundMessage", "Warning: Asset Path not valid! {0}");
			const FTextFormat MessageFormat = FTextFormat(TextFormat);
			FNotificationInfo* Info = new FNotificationInfo(FText::Format(MessageFormat, MessageIfNotFound));
			
			Info->bUseLargeFont = true;
			Info->ExpireDuration = 5.0f;
			Info->WidthOverride = 500.0f;
			Info->bUseSuccessFailIcons = true;
			Info->Image = FAppStyle::GetBrush(TEXT("Icons.Warning"));

			if (HyperlinkData.IsValid())
			{
				Info->Hyperlink = HyperlinkData.Hyperlink;
				Info->HyperlinkText = HyperlinkData.HyperlinkText;
			}
		
			FSlateNotificationManager::Get().AddNotification(*Info);
		}
	}
	
	static void TryToOpenAsset(const FString& URL, const FText& MessageIfNotFound = FText::FromString(""),
		const FHyperlinkData& HyperlinkData = FHyperlinkData() )
	{
		TryToOpenAsset(FSoftObjectPath(URL), MessageIfNotFound);
	};
}
