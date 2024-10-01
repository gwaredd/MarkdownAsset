#pragma once

#include "AssetToolsModule.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "MarkdownAssetFactoryNew.h"
#include "DeveloperSettings/MarkdownAssetDeveloperSettings.h"
#include "Shared/MarkdownAssetEditorSettings.h"
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
		if (!ObjectPath.IsValid())
		{
			return;
		}
		
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

	static FString GetAssetShortName(const UObject* Asset)
	{
		const FString BaseName = Asset->GetOutermost()->GetName();

		const FString SanitizedBasePackageName = UPackageTools::SanitizePackageName(BaseName);
		const FString PackagePath = FPackageName::GetLongPackagePath(SanitizedBasePackageName);
		return FPackageName::GetLongPackageAssetName(SanitizedBasePackageName);
	}

	static FString GetAssetNameForDocumentation(const UObject* Asset)
	{
		FString BaseAssetShortName = MarkdownAssetStatics::GetAssetShortName(Asset);

		const UMarkdownAssetEditorSettings* Settings = GetDefault<UMarkdownAssetEditorSettings>();
		if (Settings->ShouldRemovePrefix())
		{
			int32 UnderscoreIndex;
			if (BaseAssetShortName.FindChar('_', UnderscoreIndex))
			{
				// Remove all characters up to and including the underscore
				BaseAssetShortName = BaseAssetShortName.RightChop(UnderscoreIndex + 1);
			}
		}

		const UMarkdownAssetDeveloperSettings* ProjectSettings = GetDefault<UMarkdownAssetDeveloperSettings>();
		const FString FinalNameWithPrefix = ProjectSettings->GetDefaultPrefix() + BaseAssetShortName;

		return FinalNameWithPrefix;
	};

	static FString GetOrCreateDocumentationFolderPath()
	{
		const UMarkdownAssetDeveloperSettings* Settings = GetDefault<UMarkdownAssetDeveloperSettings>();
		FString RelativeDocumentationFolderPath;
		Settings->GetRelativeDocumentationFolderPath(RelativeDocumentationFolderPath);

		if (RelativeDocumentationFolderPath.IsEmpty())
		{
			return TEXT("");
		}
		
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

		/** Default folder is valid but doesn't exist, so we create it if the user wants. */
		FText Title = FText::FromString("Default Folder doesn't exist");
		FText Message = FText::Format(FText::FromString("Do you want to create the folder now '{0}'?"), FText::FromString(RelativeDocumentationFolderPath));
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
		const FString Name = MarkdownAssetStatics::GetAssetShortName(DocumentedObject);
		return FText::FromString(TEXT("# ")+Name+TEXT("\n\n"));
	};

	static UMarkdownAsset* CreateMarkdownAssetFileForAsset(const UObject* AssetToDocument)
	{
		const UMarkdownAssetDeveloperSettings* Settings = GetDefault<UMarkdownAssetDeveloperSettings>();

		const FString Prefix = Settings->GetDefaultPrefix();
		const FString Name = MarkdownAssetStatics::GetAssetNameForDocumentation(AssetToDocument);
		FString PackagePath = MarkdownAssetStatics::GetOrCreateDocumentationFolderPath();

		FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");

		/** If default folder could not be created we get the same folder as the Object to be documented. */
		if (PackagePath.IsEmpty())
		{
			FString TempName;
			AssetToolsModule.Get().CreateUniqueAssetName(AssetToDocument->GetOutermost()->GetName(), TEXT(""), PackagePath, TempName);
		}

		UMarkdownAssetFactoryNew* MarkdownFactory = NewObject<UMarkdownAssetFactoryNew>();
		const FText Content = MarkdownAssetStatics::CreateDocumentTitle(AssetToDocument);
		MarkdownFactory->Content = Content;

		// Can be null if user close the dialog or cancels.
		UObject* NewMarkdownAsset = AssetToolsModule.Get().CreateAssetWithDialog(Name, PackagePath, UMarkdownAsset::StaticClass(), MarkdownFactory);

		return Cast<UMarkdownAsset>(NewMarkdownAsset);
	}

	static void OpenOrCreateMarkdownFileForAsset(const UObject* Asset)
	{
		UMarkdownAssetDeveloperSettings* ProjectSettings = GetMutableDefault<UMarkdownAssetDeveloperSettings>();
		const FSoftObjectPath* MarkdownAsset = ProjectSettings->GetMarkdownForAsset(Asset);
		UObject* MarkdownAssetToOpen;
		
		if (MarkdownAsset->IsValid() && FPackageName::DoesPackageExist(MarkdownAsset->ToString()))
		{
			MarkdownAssetToOpen = MarkdownAsset->TryLoad();
		}
		else
		{
			MarkdownAssetToOpen = CreateMarkdownAssetFileForAsset(Asset);
			ProjectSettings->AddMarkdownAssetForFile(Asset, MarkdownAssetToOpen);
			ProjectSettings->SaveConfig(CPF_Config, *ProjectSettings->GetDefaultConfigFilename());
			
			const UMarkdownAssetEditorSettings* EditorSettings = GetDefault<UMarkdownAssetEditorSettings>();
			if(!EditorSettings->ShouldOpenNewFiles())
			{
				return;
			}
		}

		MarkdownAssetStatics::TryToOpenAsset(MarkdownAssetToOpen);
	}
}

#undef LOCTEXT_NAMESPACE