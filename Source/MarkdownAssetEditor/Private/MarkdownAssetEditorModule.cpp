#include "MarkdownAssetEditorModule.h"

#include "AnimationEditorUtils.h"
#include "Containers/Array.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"

#include "MarkdownAssetEditorSettings.h"
#include "MarkdownAssetFactoryNew.h"
#include "DeveloperSettings/MarkdownAssetDeveloperSettings.h"
#include "Toolkits/AssetEditorToolkitMenuContext.h"
#include "HelperFunctions/MarkdownAssetEditorStatics.h"
#include "Icons/Icons.h"

#define LOCTEXT_NAMESPACE "FMarkdownAssetEditorModule"

void FMarkdownAssetEditorModule::StartupModule()
{
	RegisterMenuExtensions();
	RegisterSettings();
}

void FMarkdownAssetEditorModule::ShutdownModule()
{
	UnregisterMenuExtensions();
	UnregisterSettings();
}

void FMarkdownAssetEditorModule::RegisterMenuExtensions()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	
	UToolMenu* LevelEditorToolbar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.ModesToolBar");
	
	FToolMenuSection& LevelEditorDocumentationSection = LevelEditorToolbar->FindOrAddSection("Documentation");

	LevelEditorDocumentationSection.AddEntry(FToolMenuEntry::InitToolBarButton(
		TEXT("OpenDocumentation"),
		FUIAction(FExecuteAction::CreateRaw(this, &FMarkdownAssetEditorModule::EditorAction_OpenProjectDocumentation)),
		INVTEXT("Open the project documentation."),
		INVTEXT("Open the project documentation."),
		MarkdownIcons::DocumentationIcon
	));

	
	
	UToolMenu* AssetEditorToolbar = UToolMenus::Get()->ExtendMenu("AssetEditorToolbar.CommonActions");
	
	FToolMenuSection& AssetEditorDocumentationSection = AssetEditorToolbar->FindOrAddSection("Documentation");
	AssetEditorDocumentationSection.AddDynamicEntry(NAME_None, FNewToolMenuSectionDelegate::CreateLambda(
	[this](FToolMenuSection& AssetEditorDocumentationSection)
	{
		UAssetEditorToolkitMenuContext* Context = AssetEditorDocumentationSection.FindContext<UAssetEditorToolkitMenuContext>();
	
		AssetEditorDocumentationSection.AddEntry(FToolMenuEntry::InitToolBarButton(
			TEXT("OpenAssetDocumentation"),
			FUIAction(FExecuteAction::CreateRaw(this, &FMarkdownAssetEditorModule::EditorAction_OpenAssetDocumentation, Context)),
			INVTEXT("Open asset documentation"),
			INVTEXT("Open asset documentation"),
			MarkdownIcons::DocumentationIcon
		));
	}));
}

void FMarkdownAssetEditorModule::RegisterSettings()
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

void FMarkdownAssetEditorModule::UnregisterMenuExtensions()
{
	UToolMenus::UnregisterOwner(this);
}

void FMarkdownAssetEditorModule::UnregisterSettings()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>( "Settings" );
	
	if( SettingsModule != nullptr )
	{
		SettingsModule->UnregisterSettings( "Editor", "Plugins", "MarkdownAsset" );
	}
}

void FMarkdownAssetEditorModule::EditorAction_OpenProjectDocumentation()
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
}

void FMarkdownAssetEditorModule::EditorAction_OpenAssetDocumentation(UAssetEditorToolkitMenuContext* ExecutionContext)
{
	if (!ensureAlways(ExecutionContext && ExecutionContext->GetEditingObjects().Num() > 0))
	{
		return;
	}
	
	const TArray<UObject*> Objects = ExecutionContext->GetEditingObjects();
	const UObject* Object = Objects[0];
	const UMarkdownAssetDeveloperSettings* Settings = GetDefault<UMarkdownAssetDeveloperSettings>();

	const FString Prefix = Settings->GetDefaultPrefix();
	const FString Name = MarkdownAssetStatics::GetAssetNameForDocumentation(Object);
	FString PackagePath = MarkdownAssetStatics::GetOrCreateDocumentationFolderPath();

	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");

	/** If default folder could not be created we get the same folder as the Object to be documented. */
	if (PackagePath.IsEmpty())
	{
		FString TempName;
		AssetToolsModule.Get().CreateUniqueAssetName(Object->GetOutermost()->GetName(), TEXT(""), PackagePath, TempName);
	}

	UMarkdownAssetFactoryNew* MarkdownFactory = NewObject<UMarkdownAssetFactoryNew>();
	const FText Content = MarkdownAssetStatics::CreateDocumentTitle(Object);
	MarkdownFactory->Content = Content;
	
	AssetToolsModule.Get().CreateAssetWithDialog(Name, PackagePath, UMarkdownAsset::StaticClass(), MarkdownFactory);
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE( FMarkdownAssetEditorModule, MarkdownAssetEditor );