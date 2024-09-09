#include "Containers/Array.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"

#include "MarkdownAssetEditorSettings.h"
#include "DeveloperSettings/MarkdownAssetDeveloperSettings.h"
#include "Icons/Icons.h"

#define LOCTEXT_NAMESPACE "FMarkdownAssetEditorModule"

class FMarkdownAssetEditorModule
	: public IHasMenuExtensibility
	, public IHasToolBarExtensibility
	, public IModuleInterface
{
	public:

		virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override
		{
			return MenuExtensibilityManager;
		}

	// IHasToolBarExtensibility
	public:

		virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override
		{
			return ToolBarExtensibilityManager;
		}

	// IModuleInterface
	public:

		virtual void StartupModule() override
		{
			RegisterMenuExtensions();
			RegisterSettings();
		}

		virtual void ShutdownModule() override
		{
			UnregisterMenuExtensions();
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
					LOCTEXT( "MarkdownAssetSettingsName", "Markdown Asset" ),
					LOCTEXT( "MarkdownAssetSettingsDescription", "Configure the Markdown Asset plug-in." ),
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

	protected:

		/** Registers main menu and tool bar menu extensions. */
		void RegisterMenuExtensions()
		{
			MenuExtensibilityManager = MakeShareable( new FExtensibilityManager );
			ToolBarExtensibilityManager = MakeShareable( new FExtensibilityManager );

			UToolMenu* ToolMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.ModesToolBar");
			FToolMenuSection& MenuSection = ToolMenu->FindOrAddSection("Documentation");
			MenuSection.AddEntry(FToolMenuEntry::InitToolBarButton(
				TEXT("OpenDocumentation"),
				FExecuteAction::CreateLambda([]()
				{
					const UMarkdownAssetDeveloperSettings* Settings = GetDefault<UMarkdownAssetDeveloperSettings>();
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Settings->GetDocumentationMainFileSoftPath());
				}),
				INVTEXT("Open the project documentation."),
				INVTEXT("Open the project documentation."),
				Markdown_Icons::DocumentationIcon
			));
		}

		/** Unregisters main menu and tool bar menu extensions. */
		void UnregisterMenuExtensions()
		{
			MenuExtensibilityManager.Reset();
			ToolBarExtensibilityManager.Reset();
		}

	private:

		TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
		TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
};


IMPLEMENT_MODULE( FMarkdownAssetEditorModule, MarkdownAssetEditor );
#undef LOCTEXT_NAMESPACE
