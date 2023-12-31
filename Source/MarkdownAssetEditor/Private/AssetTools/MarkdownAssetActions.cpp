#include "MarkdownAssetActions.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "MarkdownAsset.h"
#include "Styling/SlateStyle.h"
#include "MarkdownAssetEditorToolkit.h"
#include "DesktopPlatformModule.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

FMarkdownAssetActions::FMarkdownAssetActions( const TSharedRef<ISlateStyle>& InStyle )
	: Style( InStyle )
{
}


bool FMarkdownAssetActions::CanFilter()
{
	return true;
}


void FMarkdownAssetActions::GetActions( const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder )
{
	FAssetTypeActions_Base::GetActions( InObjects, MenuBuilder );

	auto MarkdownAssets = GetTypedWeakObjectPtrs<UMarkdownAsset>( InObjects );

	MenuBuilder.AddMenuEntry(
		LOCTEXT( "MarkdownAsset_Export", "Export" ),
		LOCTEXT( "MarkdownAsset_ExportToolTip", "Export the asset as a markdown (.md) file" ),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda( [=]
				{
					for( auto& MarkdownAsset : MarkdownAssets )
					{
						if( MarkdownAsset.IsValid() && !MarkdownAsset->Text.IsEmpty() )
						{
							IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

							if( DesktopPlatform )
							{
								const FString DefaultBrowsePath = FPaths::ProjectDir();
								const FString FileTypes = TEXT( "Markdown (*.md)|*.md" );

								TArray<FString> OutFilenames;

								DesktopPlatform->SaveFileDialog(
									FSlateApplication::Get().FindBestParentWindowHandleForDialogs( nullptr ),
									LOCTEXT( "StatsSaveTitle", "Save EQS stats" ).ToString(),
									DefaultBrowsePath,
									MarkdownAsset.Get()->GetName(),
									FileTypes,
									EFileDialogFlags::None,
									OutFilenames
								);

								if( OutFilenames.Num() > 0 )
								{
                                    FFileHelper::SaveStringToFile( MarkdownAsset->Text.ToString(), *OutFilenames[0] );
                                }
							}
						}
					}
				} ),
			FCanExecuteAction::CreateLambda( [=]
				{
					for( auto& MarkdownAsset : MarkdownAssets )
					{
						if( MarkdownAsset.IsValid() && !MarkdownAsset->Text.IsEmpty() )
						{
							return true;
						}
					}
					return false;
				} )
		)
	);
}


uint32 FMarkdownAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}


FText FMarkdownAssetActions::GetName() const
{
	return NSLOCTEXT( "AssetTypeActions", "AssetTypeActions_MarkdownAsset", "Markdown Asset" );
}


UClass* FMarkdownAssetActions::GetSupportedClass() const
{
	return UMarkdownAsset::StaticClass();
}


FColor FMarkdownAssetActions::GetTypeColor() const
{
	return FColor::White;
}


bool FMarkdownAssetActions::HasActions( const TArray<UObject*>& InObjects ) const
{
	return true;
}


void FMarkdownAssetActions::OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor )
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		? EToolkitMode::WorldCentric
		: EToolkitMode::Standalone;

	for( auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt )
	{
		auto MarkdownAsset = Cast<UMarkdownAsset>( *ObjIt );

		if( MarkdownAsset != nullptr )
		{
			TSharedRef<FMarkdownAssetEditorToolkit> EditorToolkit = MakeShareable( new FMarkdownAssetEditorToolkit( Style ) );
			EditorToolkit->Initialize( MarkdownAsset, Mode, EditWithinLevelEditor );
		}
	}
}

#undef LOCTEXT_NAMESPACE
