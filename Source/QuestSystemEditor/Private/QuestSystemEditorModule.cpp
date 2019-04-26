// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "QuestSystemEditorModule.h"
#include "IPluginManager.h"
#include "SlateStyleRegistry.h"
#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"
#include "AssetTypeActions_QuestInstance.h"
#include "AssetTypeActions_QuestTree.h"
#include "QuestTreeEditor.h"

#define LOCTEXT_NAMESPACE "QuestSystemEditorModule"

static TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_QuestTree;

void FQuestSystemEditorModule::StartupModule()
{
	GraphPanelNodeFactory_QuestTree = MakeShareable(new FGraphPanelNodeFactory_QuestTree());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_QuestTree);

	IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	TSharedPtr<FAssetTypeActions_QuestInstance> AssetTypeActions_QuestInstance = MakeShareable(new FAssetTypeActions_QuestInstance);
	TSharedPtr<FAssetTypeActions_QuestTree> AssetTypeActions_QuestTree = MakeShareable(new FAssetTypeActions_QuestTree);
	ItemDataAssetTypeActions.Add(AssetTypeActions_QuestInstance);
	ItemDataAssetTypeActions.Add(AssetTypeActions_QuestTree);
	AssetToolsModule.RegisterAssetTypeActions(AssetTypeActions_QuestInstance.ToSharedRef());
	AssetToolsModule.RegisterAssetTypeActions(AssetTypeActions_QuestTree.ToSharedRef());

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	StyleSet = MakeShareable(new FSlateStyleSet("QuestSystemStyle"));
	FString BaseDir = IPluginManager::Get().FindPlugin("QuestSystem")->GetBaseDir();

	StyleSet->SetContentRoot(BaseDir);

	//Create a brush from the icon
	FSlateImageBrush* ThumbnailBrush16 = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/Icons/BehaviorTree_16x"), TEXT(".png")), FVector2D(16.f, 16.f));
	FSlateImageBrush* ThumbnailBrush64 = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/Icons/BehaviorTree_64x"), TEXT(".png")), FVector2D(64.f, 64.f));

	//In order to bind the thumbnail to our class we need to type ClassThumbnail.X where X is the name of the C++ class of the asset
	StyleSet->Set("ClassIcon.QuestTree", ThumbnailBrush16);
	StyleSet->Set("ClassThumbnail.QuestTree", ThumbnailBrush64);

	//Reguster the created style
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);

}

void FQuestSystemEditorModule::ShutdownModule()
{
	if (GraphPanelNodeFactory_QuestTree.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_QuestTree);
		GraphPanelNodeFactory_QuestTree.Reset();
	}

	// Unregister the item data asset type actions
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto& AssetTypeAction : ItemDataAssetTypeActions)
		{
			if (AssetTypeAction.IsValid())
			{
				AssetToolsModule.UnregisterAssetTypeActions(AssetTypeAction.ToSharedRef());
			}
		}
	}
	ItemDataAssetTypeActions.Empty();

	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemEditor)