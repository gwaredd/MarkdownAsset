// Copyright (C) 2024 Gwaredd Mountain - All Rights Reserved.

#pragma once

#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"
#include "MarkdownAssetFactoryNew.generated.h"

UCLASS( hidecategories = Object )
class UMarkdownAssetFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	
	virtual UObject* FactoryCreateNew( UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn ) override;
	virtual bool ShouldShowInNewMenu() const override;

	FText Content;
};
