#include "Containers/Array.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"

#include "MarkdownAssetEditorSettings.h"
#include "DeveloperSettings/MarkdownAssetDeveloperSettings.h"
#include "HelperFunctions/MarkdownAssetEditorStatics.h"
#include "Icons/Icons.h"

#define LOCTEXT_NAMESPACE "FMarkdownAssetEditorModule"

class FMarkdownAssetEditorModule : public IModuleInterface
{
	// IModuleInterface
	public:

		virtual void StartupModule() override
		{
			RegisterMenuExtensions();
			RegisterSettings();
		}

		virtual void ShutdownModule() override
		{
			//UnregisterMenuExtensions();
			UnregisterSettings();
		}

		virtual bool SupportsDynamicReloading() override
		{
			return true;
		}

	protected:
	
		void RegisterSettings()
		{
			ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>( "Settings" );

			if( SettingsModule != nullptr )
			{
				ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings( "Editor", "Plugins", "MarkdownAsset",
					LOCTEXT("MarkdownAssetSettingsName", "Markdown Asset" ),
					LOCTEXT("MarkdownAssetSettingsDescription", "Configure the Markdown Asset plug-in." ),
					GetMutableDefault<UMarkdownAssetEditorSettings>()
				);
			}
		}

		void UnregisterSettings()
		{
			ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>( "Settings" );
			
			if( SettingsModule != nullptr )
			{
				SettingsModule->UnregisterSettings( "Editor", "Plugins", "MarkdownAsset" );
			}
		}
	
		/** Registers main menu and tool bar menu extensions. */
		void RegisterMenuExtensions()
		{
			UToolMenu* ToolMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.ModesToolBar");
			FToolMenuSection& MenuSection = ToolMenu->FindOrAddSection("Documentation");
			MenuSection.AddEntry(FToolMenuEntry::InitToolBarButton(
				TEXT("OpenDocumentation"),
				FExecuteAction::CreateLambda([]()
				{
					const FText NotFoundMessage = LOCTEXT("MarkdownAssetProjectMainFileNotFound", "Define your main file in the MarkdownAsset Settings Screen!");
					const UMarkdownAssetDeveloperSettings* Settings = GetDefault<UMarkdownAssetDeveloperSettings>();

					MarkdownAssetStatics::FHyperlinkData HyperlinkData;
					HyperlinkData.Hyperlink = FSimpleDelegate::CreateLambda([]()
					{
						UMarkdownAssetDeveloperSettings::Get()->OpenEditorSettingWindow();
					});
					HyperlinkData.HyperlinkText = NotFoundMessage;
					
					MarkdownAssetStatics::TryToOpenAsset(Settings->GetDocumentationMainFileSoftPath(), FText::FromString(""), HyperlinkData);
				}),
				INVTEXT("Open the project documentation."),
				INVTEXT("Open the project documentation."),
				MarkdownIcons::DocumentationIcon
			));
		}
};


IMPLEMENT_MODULE( FMarkdownAssetEditorModule, MarkdownAssetEditor );
#undef LOCTEXT_NAMESPACE
