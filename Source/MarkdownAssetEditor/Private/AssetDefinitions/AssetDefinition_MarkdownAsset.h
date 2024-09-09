// Copyright (C) 2024 El Burro Games - All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "AssetDefinition_MarkdownAsset.generated.h"

/**
 * 
 */
UCLASS()
class MARKDOWNASSETEDITOR_API UAssetDefinition_MarkdownAsset : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:

	virtual TSoftClassPtr<UObject> GetAssetClass() const override;

	virtual FText GetAssetDisplayName() const override;

	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;

	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;

	virtual FLinearColor GetAssetColor() const override;

	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
};
