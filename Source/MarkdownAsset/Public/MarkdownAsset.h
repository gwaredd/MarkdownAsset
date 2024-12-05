// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

#pragma once

#include "Internationalization/Text.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "MarkdownAsset.generated.h"


UCLASS( BlueprintType, hidecategories = ( Object ) )
class MARKDOWNASSET_API UMarkdownAsset : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "MarkdownAsset" )
	FText Text;
};
