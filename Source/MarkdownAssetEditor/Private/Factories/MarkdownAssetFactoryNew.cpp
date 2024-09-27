#include "MarkdownAssetFactoryNew.h"
#include "MarkdownAsset.h"

UMarkdownAssetFactoryNew::UMarkdownAssetFactoryNew( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
	SupportedClass = UMarkdownAsset::StaticClass();
	bCreateNew     = true;
	bEditAfterNew  = true;
}

UObject* UMarkdownAssetFactoryNew::FactoryCreateNew( UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn )
{
	UMarkdownAsset* MarkdownAsset = NewObject<UMarkdownAsset>( InParent, InClass, InName, Flags | RF_Transactional );
	MarkdownAsset->Text = Content;
	return MarkdownAsset;
}

bool UMarkdownAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}
