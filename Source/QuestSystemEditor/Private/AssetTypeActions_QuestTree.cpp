// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_QuestTree.h"
#include "Framework/Application/SlateApplication.h"
#include "QuestTree.h"
#include "QuestTreeEditor.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

uint32 FAssetTypeActions_QuestTree::GetCategories()
{ 
	return EAssetTypeCategories::Gameplay;
}

UClass* FAssetTypeActions_QuestTree::GetSupportedClass() const
{ 
	return UQuestTree::StaticClass(); 
}

void FAssetTypeActions_QuestTree::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>() */)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UQuestTree* Graph = Cast<UQuestTree>(*ObjIt))
		{
			TSharedRef<FQuestTreeEditor> NewGraphEditor(new FQuestTreeEditor());
			NewGraphEditor->InitObjectiveGraphEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}


#undef LOCTEXT_NAMESPACE
