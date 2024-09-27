#pragma once
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "DeveloperSettings/MarkdownAssetDeveloperSettings.h"
#include "Framework/Notifications/NotificationManager.h"
#include "LogChannels/MarkdownLogChannels.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FMarkdownAssetEditorStaticFunctions"

namespace MarkdownAssetStatics
{
	static FString DefaultDocumentationFolderName = TEXT("Documentation");

	inline FString ConvertProjectPathToAbsolutePath(const FString& LocalPath)
	{
		const FString Prefix = TEXT("/Game/");
		const FString RightChop = LocalPath.RightChop(Prefix.Len());
		return FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / RightChop);
	};
	
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
	};
	
	static void TryToOpenAsset(const FString& URL, const FText& MessageIfNotFound = FText::FromString(""),
		const FHyperlinkData& HyperlinkData = FHyperlinkData() )
	{
		TryToOpenAsset(FSoftObjectPath(URL), MessageIfNotFound);
	};

	static FString GetAssetNameForDocumentation(const UObject* Asset)
	{
		const FString BaseName = Asset->GetOutermost()->GetName();

		const FString SanitizedBasePackageName = UPackageTools::SanitizePackageName(BaseName);
		const FString PackagePath = FPackageName::GetLongPackagePath(SanitizedBasePackageName);
		FString BaseAssetShortName = FPackageName::GetLongPackageAssetName(SanitizedBasePackageName);

		const UMarkdownAssetDeveloperSettings* Settings = GetDefault<UMarkdownAssetDeveloperSettings>();
		if (Settings->ShouldRemovePrefix())
		{
			int32 UnderscoreIndex;
			if (BaseAssetShortName.FindChar('_', UnderscoreIndex))
			{
				// Remove all characters up to and including the underscore
				BaseAssetShortName = BaseAssetShortName.RightChop(UnderscoreIndex + 1);
			}
		}

		const FString FinalNameWithPrefix = Settings->GetDefaultPrefix() + BaseAssetShortName;

		return FinalNameWithPrefix;
	};

	static FString GetOrCreateDocumentationFolderPath()
	{
		const UMarkdownAssetDeveloperSettings* Settings = GetDefault<UMarkdownAssetDeveloperSettings>();
		const FString RelativeDocumentationFolderPath = Settings->GetRelativeDocumentationFolderPath();

		FString AbsolutePath;
		
		if (!FPackageName::TryConvertLongPackageNameToFilename(RelativeDocumentationFolderPath, AbsolutePath))
		{
			UE_LOG(MarkdownStaticsLog, Type::Error, TEXT("GetDocumentationFolderPath Failed - Failed to convert the path '%s' set in settings to absolute."), *RelativeDocumentationFolderPath);
			return TEXT("");
		}

		if (IFileManager::Get().DirectoryExists(*AbsolutePath))
		{
			return RelativeDocumentationFolderPath;
		}
		
		FText Title = FText::FromString("Default Folder doesn't exist");
		FText Message = FText::Format(FText::FromString("Do you want to create the folder '{0}'?"), FText::FromString(RelativeDocumentationFolderPath));
		EAppReturnType::Type Response = FMessageDialog::Open(EAppMsgType::YesNo, Message, Title);

		if (Response == EAppReturnType::No)
		{
			return TEXT("");
		}

		if (!IFileManager::Get().MakeDirectory(*AbsolutePath, true))
		{
			UE_LOG(MarkdownStaticsLog, Type::Error, TEXT("GetDocumentationFolderPath Failed - Failed to create folder '%s'"), *AbsolutePath);
			return TEXT("");
		}
		
		/** We add the new folder to the CachedFolders so can be used inmediatly. */
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		AssetRegistryModule.Get().AddPath(RelativeDocumentationFolderPath);
		
		return RelativeDocumentationFolderPath;
	};

	static FText CreateDocumentTitle(const UObject* DocumentedObject)
	{
		return FText::FromString(TEXT("# ")+DocumentedObject->GetOutermost()->GetName()+TEXT("\n\n"));
	}
}

#undef LOCTEXT_NAMESPACE