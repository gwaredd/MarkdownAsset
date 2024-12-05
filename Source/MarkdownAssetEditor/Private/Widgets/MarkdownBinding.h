// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MarkdownBinding.generated.h"

UCLASS()
class MARKDOWNASSETEDITOR_API UMarkdownBinding : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION()
	FText GetText() { return Text; }

	UFUNCTION()
	void SetText( FText text ) { Text = text; OnSetText.Broadcast(); }

	UFUNCTION()
	void OpenURL( FString url );

	UFUNCTION()
	void OpenAsset( FString url );

	DECLARE_EVENT( UMarkdownBinding, FOnSetTextEvent )
	FOnSetTextEvent OnSetText;

	FText Text;
};
