#include "MarkdownAssetActions.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "MarkdownAsset.h"
#include "Styling/SlateStyle.h"
#include "MarkdownAssetEditorToolkit.h"


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
