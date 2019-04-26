// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_QuestInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "QuestInstance.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

uint32 FAssetTypeActions_QuestInstance::GetCategories()
{ 
	return EAssetTypeCategories::Gameplay;
}

UClass* FAssetTypeActions_QuestInstance::GetSupportedClass() const
{ 
	return UQuestInstance::StaticClass(); 
}

#undef LOCTEXT_NAMESPACE
