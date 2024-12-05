// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

#pragma once

#include "EditorUndoClient.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "UObject/GCObject.h"

class FSpawnTabArgs;
class ISlateStyle;
class IToolkitHost;
class SDockTab;
class UMarkdownAsset;

class FMarkdownAssetEditorToolkit : public FAssetEditorToolkit, public FGCObject
{
	public:

		// FMarkdownAssetEditorToolkit( const TSharedRef<ISlateStyle>& InStyle );
		FMarkdownAssetEditorToolkit();
		virtual ~FMarkdownAssetEditorToolkit();

		static const TSharedPtr<ISlateStyle> Style;

	public:

		void Initialize( UMarkdownAsset* InMarkdownAsset, const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InToolkitHost );

		//~ FAssetEditorToolkit interface
		virtual FString GetDocumentationLink() const override;
		virtual void RegisterTabSpawners( const TSharedRef<FTabManager>& InTabManager ) override;
		virtual void UnregisterTabSpawners( const TSharedRef<FTabManager>& InTabManager ) override;

		//~ IToolkit interface
		virtual FText GetBaseToolkitName() const override;
		virtual FName GetToolkitFName() const override;
		virtual FLinearColor GetWorldCentricTabColorScale() const override;
		virtual FString GetWorldCentricTabPrefix() const override;

		//~ FGCObject interface
		virtual void AddReferencedObjects( FReferenceCollector& Collector ) override;

		virtual FString GetReferencerName() const
		{
			return TEXT( "FMarkdownAssetEditorToolkit" );
		}

	private:

		TSharedRef<SDockTab> HandleTabManagerSpawnTab( const FSpawnTabArgs& Args, FName TabIdentifier );

	private:
		TObjectPtr<UMarkdownAsset> MarkdownAsset;
};
