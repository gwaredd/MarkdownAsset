#include "MarkdownAssetFactory.h"

#include "Containers/UnrealString.h"
#include "MarkdownAsset.h"
#include "Misc/FileHelper.h"


UMarkdownAssetFactory::UMarkdownAssetFactory( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
	Formats.Add( TEXT( "md;Markdown" ) );
	SupportedClass = UMarkdownAsset::StaticClass();
	bCreateNew     = false;
	bEditorImport  = true;
}

UObject* UMarkdownAssetFactory::FactoryCreateFile( UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled )
{
	UMarkdownAsset* MarkdownAsset = nullptr;
	FString TextString;

	if( FFileHelper::LoadFileToString( TextString, *Filename ) )
	{
		MarkdownAsset = NewObject<UMarkdownAsset>( InParent, InClass, InName, Flags );
		MarkdownAsset->Text = FText::FromString( TextString );
	}

	bOutOperationCanceled = false;

	return MarkdownAsset;
}
