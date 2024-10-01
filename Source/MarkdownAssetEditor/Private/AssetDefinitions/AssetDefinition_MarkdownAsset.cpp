// Copyright (C) 2024 El Burro Games - All Rights Reserved.


#include "AssetDefinition_MarkdownAsset.h"

#include "ContentBrowserMenuContexts.h"
#include "DesktopPlatformModule.h"
#include "MarkdownAsset.h"
#include "MarkdownAssetEditorToolkit.h"
#include "Icons/Icons.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

namespace MarkdownMenuNames
{
	const FName MenuCustomActionsSectionName = TEXT("Markdown");
	const FName ExportAsMDActionName = TEXT("ExportAsMDFile");
}

TSoftClassPtr<UObject> UAssetDefinition_MarkdownAsset::GetAssetClass() const
{
	return UMarkdownAsset::StaticClass();
}

FText UAssetDefinition_MarkdownAsset::GetAssetDisplayName() const
{
	return NSLOCTEXT("AssetTypeActions", "MarkdownAsset_AssetDefaultDisplayName", "Markdown Asset");
}

FText UAssetDefinition_MarkdownAsset::GetAssetDescription(const FAssetData& AssetData) const
{
	return NSLOCTEXT("AssetTypeActions", "MarkdownAsset_AssetDefaultDescription", "A Markdown asset.");
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_MarkdownAsset::GetAssetCategories() const
{
	static const FAssetCategoryPath Categories[] = { FText::FromString("Documentation") };
	return Categories;
}

FLinearColor UAssetDefinition_MarkdownAsset::GetAssetColor() const
{
	return FLinearColor::White;
}

EAssetCommandResult UAssetDefinition_MarkdownAsset::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	for (UMarkdownAsset* MarkdownAsset : OpenArgs.LoadObjects<UMarkdownAsset>())
	{
		if (MarkdownAsset)
		{
			TSharedRef<FMarkdownAssetEditorToolkit> EditorToolkit = MakeShareable(new FMarkdownAssetEditorToolkit());
			EditorToolkit->Initialize(MarkdownAsset, OpenArgs.GetToolkitMode(), OpenArgs.ToolkitHost);
		}
	}

	return EAssetCommandResult::Handled;
}

// Menu Extensions
//--------------------------------------------------------------------

namespace MenuExtension_MarkdownAsset
{
	void ExecuteExportAsMarkdownFile(const FToolMenuContext& InContext)
	{
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

		const UContentBrowserAssetContextMenuContext* Context = UContentBrowserAssetContextMenuContext::FindContextWithAssets(InContext);
		for (UMarkdownAsset* MarkdownAsset : Context->LoadSelectedObjects<UMarkdownAsset>())
		{
			if (MarkdownAsset && !MarkdownAsset->Text.IsEmpty())
			{
				if (DesktopPlatform)
				{
					const FText Title = FText::Format(LOCTEXT("MarkdownAsset_ExportMDDialogTitle", "Export '{0}' as Markdown..."), FText::FromString(*MarkdownAsset->GetName()));
					const FString DefaultBrowsePath = FPaths::ProjectDir();
					const FString FileTypes = TEXT( "Markdown (*.md)|*.md" );

					// TODO (Maxi): When we add support to import actual .md files we can retriver the path for the original asset from here.
					// const FString CurrentFilename = MarkdownAsset->AssetImportData->GetFirstFilename();
					// const FString DefaultBrowsePath = (CurrentFilename.IsEmpty()) ? TEXT("") : FPaths::GetPath(CurrentFilename),;
					
					TArray<FString> OutFilenames;
					DesktopPlatform->SaveFileDialog(
						ParentWindowWindowHandle,
						Title.ToString(),
						DefaultBrowsePath,
						MarkdownAsset->GetName(),
						FileTypes,
						EFileDialogFlags::None,
						OutFilenames
					);

					if (OutFilenames.Num() > 0)
					{
						FFileHelper::SaveStringToFile(MarkdownAsset->Text.ToString(), *OutFilenames[0]);
					}
				}
			}
		}
	}

	static FDelayedAutoRegisterHelper DelayedAutoRegister(EDelayedRegisterRunPhase::EndOfEngineInit, []{ 
		UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([]()
		{
			FToolMenuOwnerScoped OwnerScoped(UE_MODULE_NAME);
			UToolMenu* Menu = UE::ContentBrowser::ExtendToolMenu_AssetContextMenu(UMarkdownAsset::StaticClass());
		
			FToolMenuSection& Section = Menu->FindOrAddSection(MarkdownMenuNames::MenuCustomActionsSectionName);
			Section.AddSeparator(MarkdownMenuNames::MenuCustomActionsSectionName);
			Section.AddDynamicEntry(MarkdownMenuNames::ExportAsMDActionName, FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
			{
				{
					const TAttribute<FText> Label = LOCTEXT("MarkdownAsset_ExportAsMD", "Export as MD File");
					const TAttribute<FText> ToolTip = LOCTEXT("MarkdownAsset_ExportAsMDTooltip", "Export the MarkdownAsset as a .MD file.");
					const FSlateIcon Icon = MarkdownIcons::DocumentationIcon;

					FToolUIAction UIAction = FToolMenuExecuteAction::CreateStatic(&ExecuteExportAsMarkdownFile);
					InSection.AddMenuEntry("MarkdownAsset_ExportAsMD", Label, ToolTip, Icon, UIAction);
				}
			}));
		}));
	});
}

#undef LOCTEXT_NAMESPACE
