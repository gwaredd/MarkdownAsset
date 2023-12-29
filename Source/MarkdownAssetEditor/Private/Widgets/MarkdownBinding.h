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

	DECLARE_EVENT( UMarkdownBinding, FOnSetTextEvent )
	FOnSetTextEvent OnSetText;

	FText Text;

};
