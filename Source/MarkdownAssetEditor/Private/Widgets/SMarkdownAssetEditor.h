#pragma once

#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "SWebBrowser.h"

class FText;
class ISlateStyle;
class UMarkdownAsset;

class SMarkdownAssetEditor : public SCompoundWidget
{
	public:

		SLATE_BEGIN_ARGS( SMarkdownAssetEditor ) {}
		SLATE_END_ARGS()

	public:

		virtual ~SMarkdownAssetEditor();

		void Construct( const FArguments& InArgs, UMarkdownAsset* InMarkdownAsset, const TSharedRef<ISlateStyle>& InStyle );

	private:

		void HandleMarkdownAssetPropertyChanged( UObject* Object, FPropertyChangedEvent& PropertyChangedEvent );
		void HandleConsoleMessage( const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Serverity );

	private:

		TSharedPtr<SWebBrowserView> WebBrowser;
		UMarkdownAsset* MarkdownAsset;
};
