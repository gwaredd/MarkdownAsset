#include "MarkdownAssetEditorToolkit.h"
#include "Editor.h"
#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "SMarkdownAssetEditor.h"
#include "MarkdownAsset.h"
#include "UObject/NameTypes.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FMarkdownAssetEditorToolkit"

///////////////////////////////////////////////////////////////////////////////

namespace MarkdownAssetEditor
{
	static const FName AppIdentifier( "MarkdownAssetEditorApp" );
	static const FName TabId( "MarkdownEditor" );
}

///////////////////////////////////////////////////////////////////////////////

FMarkdownAssetEditorToolkit::FMarkdownAssetEditorToolkit( const TSharedRef<ISlateStyle>& InStyle )
	: MarkdownAsset( nullptr )
	, Style( InStyle )
{
}

FMarkdownAssetEditorToolkit::~FMarkdownAssetEditorToolkit()
{
	FReimportManager::Instance()->OnPreReimport().RemoveAll( this );
	FReimportManager::Instance()->OnPostReimport().RemoveAll( this );
}


///////////////////////////////////////////////////////////////////////////////

void FMarkdownAssetEditorToolkit::Initialize( UMarkdownAsset* InMarkdownAsset, const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost )
{
	MarkdownAsset = InMarkdownAsset;

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout( "Standalone_MarkdownAssetEditor_v1.3" )
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation( Orient_Horizontal )
			->Split
			(
				FTabManager::NewStack()
				->AddTab( MarkdownAssetEditor::TabId, ETabState::OpenedTab )
				->SetHideTabWell( true )
				->SetSizeCoefficient( 1.0f )
			)
		);

	FAssetEditorToolkit::InitAssetEditor(
		InMode,
		InToolkitHost,
		MarkdownAssetEditor::AppIdentifier,
		Layout,
		true /*bCreateDefaultStandaloneMenu*/,
		true /*bCreateDefaultToolbar*/,
		InMarkdownAsset
	);

	RegenerateMenusAndToolbars();
}

///////////////////////////////////////////////////////////////////////////////

FString FMarkdownAssetEditorToolkit::GetDocumentationLink() const
{
	return FString( TEXT( "https://github.com/gwaredd" ) );
}

void FMarkdownAssetEditorToolkit::RegisterTabSpawners( const TSharedRef<FTabManager>& InTabManager )
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory( LOCTEXT( "WorkspaceMenu_MarkdownAssetEditor", "Markdown Asset Editor" ) );
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners( InTabManager );

	InTabManager->RegisterTabSpawner( MarkdownAssetEditor::TabId, FOnSpawnTab::CreateSP( this, &FMarkdownAssetEditorToolkit::HandleTabManagerSpawnTab, MarkdownAssetEditor::TabId ) )
		.SetDisplayName( LOCTEXT( "MarkdownEditorTabName", "Markdown Editor" ) )
		.SetGroup( WorkspaceMenuCategoryRef )
		.SetIcon( FSlateIcon( FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewports" ) )
		;
}

void FMarkdownAssetEditorToolkit::UnregisterTabSpawners( const TSharedRef<FTabManager>& InTabManager )
{
	FAssetEditorToolkit::UnregisterTabSpawners( InTabManager );
	InTabManager->UnregisterTabSpawner( MarkdownAssetEditor::TabId );
}

FText FMarkdownAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT( "AppLabel", "Markdown Asset Editor" );
}

FName FMarkdownAssetEditorToolkit::GetToolkitFName() const
{
	return FName( "MarkdownAssetEditor" );
}

FLinearColor FMarkdownAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor( 0.3f, 0.2f, 0.5f, 0.5f );
}

FString FMarkdownAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT( "WorldCentricTabPrefix", "MarkdownAsset " ).ToString();
}

void FMarkdownAssetEditorToolkit::AddReferencedObjects( FReferenceCollector& Collector )
{
	Collector.AddReferencedObject( MarkdownAsset );
}

TSharedRef<SDockTab> FMarkdownAssetEditorToolkit::HandleTabManagerSpawnTab( const FSpawnTabArgs& Args, FName TabIdentifier )
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;

	if( TabIdentifier == MarkdownAssetEditor::TabId )
	{
		TabWidget = SNew( SMarkdownAssetEditor, MarkdownAsset, Style );
	}

	return SNew( SDockTab )
		.TabRole( ETabRole::PanelTab )
		[
			TabWidget.ToSharedRef()
		];
}

#undef LOCTEXT_NAMESPACE
