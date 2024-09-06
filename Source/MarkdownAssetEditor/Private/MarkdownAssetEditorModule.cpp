#include "Containers/Array.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"

#include "AssetTools/MarkdownAssetActions.h"
#include "Styles/MarkdownAssetEditorStyle.h"
#include "MarkdownAssetEditorSettings.h"
#include "DeveloperSettings/MarkdownAssetDeveloperSettings.h"

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
			Style = MakeShareable( new FMarkdownAssetEditorStyle() );

			RegisterAssetTools();
			RegisterMenuExtensions();
			RegisterSettings();
		}

		virtual void ShutdownModule() override
		{
			UnregisterAssetTools();
			UnregisterMenuExtensions();
			UnregisterSettings();
		}

		virtual bool SupportsDynamicReloading() override
		{
			return true;
		}

	protected:

		void RegisterAssetTools()
		{
			IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>( "AssetTools" ).Get();
			RegisterAssetTypeAction( AssetTools, MakeShareable( new FMarkdownAssetActions( Style.ToSharedRef() ) ) );
		}

		void RegisterAssetTypeAction( IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action )
		{
			AssetTools.RegisterAssetTypeActions( Action );
			RegisteredAssetTypeActions.Add( Action );
		}

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

		void UnregisterAssetTools()
		{
			FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>( "AssetTools" );

			if( AssetToolsModule != nullptr )
			{
				IAssetTools& AssetTools = AssetToolsModule->Get();

				for( auto Action : RegisteredAssetTypeActions )
				{
					AssetTools.UnregisterAssetTypeActions( Action );
				}
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
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Documentation")
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
		TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
		TSharedPtr<ISlateStyle> Style;
		TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
};


IMPLEMENT_MODULE( FMarkdownAssetEditorModule, MarkdownAssetEditor );
#undef LOCTEXT_NAMESPACE
