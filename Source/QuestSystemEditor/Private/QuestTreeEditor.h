#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "GraphEditor.h"
#include "EditorUndoClient.h"
#include "EdGraphUtilities.h"

class FGraphPanelNodeFactory_QuestTree : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};

class FQuestTreeEditor : public FAssetEditorToolkit, public FEditorUndoClient
{
public:
	FQuestTreeEditor();

	// FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	// End of FAssetEditorToolkit

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	virtual void PostUndo(bool bSuccess) override;

public:
	void InitObjectiveGraphEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UObject* InObject);

public:
	TSharedRef<SDockTab> SpawnTab_GraphEditor(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_DetailsTap(const FSpawnTabArgs& Args);

	bool CanDeleteNodes();
	void DeleteSelectedNodes();

private:
	void CreateCommandList();
	void CreateGraphEditor(class UQuestTree* GraphToEdit);

private:
	TSharedPtr<SGraphEditor> GraphEditor;
	TSharedPtr<FUICommandList> EditorCommands;

public:
	static const FName GraphEditorID;
	static const FName DetailsID;
	static const FName EditorAppIdentifier;
};
