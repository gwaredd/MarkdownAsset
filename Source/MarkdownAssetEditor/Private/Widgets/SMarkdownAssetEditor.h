// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

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
		virtual FReply OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent ) override;

	private:

		void HandleMarkdownAssetPropertyChanged( UObject* Object, FPropertyChangedEvent& PropertyChangedEvent );
		void HandleConsoleMessage( const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Serverity );
		bool OnLoadURL( const FString& Method, const FString& Url, FString& Response );


	private:

		TSharedPtr<SWebBrowserView> WebBrowser;
		UMarkdownAsset* MarkdownAsset;
};
