#include "QuestTreeEditor.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "QuestTree.h"
#include "EdGraphSchema_QuestTree.h"
#include "GenericCommands.h"
#include "ScopedTransaction.h"

#include "EdGraphNode_QuestTree.h"
#include "EdGraphNode_QuestInstance.h"
#include "SGraphNode_QuestInstance.h"
#include "SGraphNode_QuestTree.h"

#define LOCTEXT_NAMESPACE "ObjectiveGraphEditor"

TSharedPtr<class SGraphNode> FGraphPanelNodeFactory_QuestTree::CreateNode(UEdGraphNode* Node) const
{

	if (UEdGraphNode_QuestTree* GraphNode = Cast<UEdGraphNode_QuestTree>(Node))
	{
		return SNew(SGraphNode_QuestTree, GraphNode);
	}
	else if (UEdGraphNode_QuestInstance* GraphNode = Cast<UEdGraphNode_QuestInstance>(Node))
	{
		return SNew(SGraphNode_QuestInstance, GraphNode);
	}
	return nullptr;
}

const FName FQuestTreeEditor::EditorAppIdentifier(TEXT("QuestTreeEditorApp"));
const FName FQuestTreeEditor::GraphEditorID(TEXT("QuestTreeEditor_GraphEditor"));
const FName FQuestTreeEditor::DetailsID(TEXT("QuestTreeEditor_Details"));

FQuestTreeEditor::FQuestTreeEditor()
{
	UEditorEngine* Editor = (UEditorEngine*)GEngine;
	if (Editor != NULL)
	{
		Editor->RegisterForUndo(this);
	}
}

void FQuestTreeEditor::PostUndo(bool bSuccess)
{
	if (bSuccess)
	{
		GraphEditor->NotifyGraphChanged();
	}

	FEditorUndoClient::PostUndo(bSuccess);
}

FName FQuestTreeEditor::GetToolkitFName() const
{
	return FName("Quest Tree");
}

FText FQuestTreeEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "QuestTree");
}

FLinearColor FQuestTreeEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.0f, 0.0f, 0.2f, 0.5f);
}

FString FQuestTreeEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "QuestTree").ToString();
}


void FQuestTreeEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(TabManager);

	TabManager->RegisterTabSpawner(FQuestTreeEditor::GraphEditorID, FOnSpawnTab::CreateSP(this, &FQuestTreeEditor::SpawnTab_GraphEditor))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "ObjectiveGraph"));

	TabManager->RegisterTabSpawner(FQuestTreeEditor::DetailsID, FOnSpawnTab::CreateSP(this, &FQuestTreeEditor::SpawnTab_DetailsTap))
		.SetDisplayName(LOCTEXT("DetailsTab", "Details"));
}


void FQuestTreeEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(TabManager);

	TabManager->UnregisterTabSpawner(FQuestTreeEditor::GraphEditorID);
	TabManager->UnregisterTabSpawner(FQuestTreeEditor::DetailsID);
}

void FQuestTreeEditor::InitObjectiveGraphEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UObject* ObjectToEdit)
{
	FGenericCommands::Register();

	UQuestTree* GraphToEdit = Cast<UQuestTree>(ObjectToEdit);

	CreateGraphEditor(GraphToEdit);

	const TSharedRef<FTabManager::FLayout> DummyLayout = FTabManager::NewLayout("Standalone_ObjeciveGraph_Layout_v1")->AddArea(
		FTabManager::NewPrimaryArea()->SetOrientation(EOrientation::Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
		)
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.7f)
				->AddTab(FQuestTreeEditor::GraphEditorID, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.3f)
				->AddTab(FQuestTreeEditor::DetailsID, ETabState::OpenedTab)->SetHideTabWell(true)
			)
		)
	);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;

	InitAssetEditor(Mode, InitToolkitHost, FQuestTreeEditor::EditorAppIdentifier, DummyLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectToEdit);

	//RegenerateMenusAndToolbars();
}

TSharedRef<SDockTab> FQuestTreeEditor::SpawnTab_GraphEditor(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("ObjectiveGraph_Title", "ObjectiveGraph"));

	if (GraphEditor.IsValid())
	{
		SpawnedTab->SetContent(GraphEditor.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FQuestTreeEditor::SpawnTab_DetailsTap(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("Details_Title", "Details"));
	/*
			[
				PropertyWidget.ToSharedRef()
			];*/
}

void FQuestTreeEditor::CreateGraphEditor(UQuestTree* GraphToEdit)
{
	if (!IsValid(GraphToEdit->ObjGraph))
	{
		GraphToEdit->ObjGraph = FBlueprintEditorUtils::CreateNewGraph(GraphToEdit, TEXT("Quest Tree"), UEdGraph::StaticClass(), UEdGraphSchema_QuestTree::StaticClass());
	}

	CreateCommandList();

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_QuestTree", "Quest Tree");

	// Make title bar
	TSharedRef<SWidget> TitleBarWidget =
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.FillWidth(1.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ObjectiveGraphLabel", "Objective Graph"))
		.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
		]
		];

	SGraphEditor::FGraphEditorEvents InEvents;

	GraphEditor = SNew(SGraphEditor)
		.IsEditable(true)
		.GraphToEdit(GraphToEdit->ObjGraph)
		.AdditionalCommands(EditorCommands)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBarWidget)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

bool FQuestTreeEditor::CanDeleteNodes()
{
	return true;
}

void FQuestTreeEditor::CreateCommandList()
{
	if (!EditorCommands.IsValid())
	{
		EditorCommands = MakeShareable(new FUICommandList);
	}

	EditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FQuestTreeEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FQuestTreeEditor::CanDeleteNodes)
	);
}

void FQuestTreeEditor::DeleteSelectedNodes()
{
	if (!GraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	GraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();
	GraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())
			continue;;

		if (UEdGraphNode_QuestTree* EdNode_Node = Cast<UEdGraphNode_QuestTree>(EdNode))
		{
			EdNode_Node->Modify();

			const UEdGraphSchema* Schema = EdNode_Node->GetSchema();
			if (Schema != nullptr)
			{
				//@todo
				//Schema->BreakNodeLinks(*EdNode_Node);
			}

			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

#undef LOCTEXT_NAMESPACE
