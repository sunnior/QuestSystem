// Fill out your copyright notice in the Description page of Project Settings.

#include "EdGraphSchema_QuestTree.h"
#include "EdGraph/EdGraph.h"
#include "Slate.h"
#include "GraphEditorActions.h"
#include "ScopedTransaction.h"
#include "QuestTree.h"
#include "QuestInstance.h"
#include "EdGraphNode_QuestTree.h"
#include "EdGraphNode_QuestInstance.h"

#define LOCTEXT_NAMESPACE "QuestTreeEditor"

UEdGraphNode* FEdQuestTreeSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
	UEdGraphNode* ResultNode = nullptr;

	if (NodeTemplate != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("QuestTreeEditorNewNode", "Quest Tree: New Node"));

		ParentGraph->Modify();
		if (FromPin != nullptr)
			FromPin->Modify();

		NodeTemplate->SetFlags(RF_Transactional);

		// set outer to be the graph so it doesn't go away
		NodeTemplate->Rename(NULL, ParentGraph, REN_NonTransactional);
		ParentGraph->AddNode(NodeTemplate, true);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();

		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void FEdQuestTreeSchemaAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

void UEdGraphSchema_QuestTree::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	FCategorizedGraphActionListBuilder ActionsBuilder(TEXT("QuestTree"));

	const FText AddToolTip = LOCTEXT("QuestTreeNewNodeTooltip", "Add node here");
	const FText Desc = LOCTEXT("QuestTreeNewNodeDesc", "Add node here Desc");

	TSharedPtr<FEdQuestTreeSchemaAction_NewNode> NewNodeAction(new FEdQuestTreeSchemaAction_NewNode(FText::GetEmpty(), Desc, AddToolTip, 0));
	UEdGraphNode_QuestTree* OpNode = NewObject<UEdGraphNode_QuestTree>(ContextMenuBuilder.OwnerOfTemporaries, UEdGraphNode_QuestTree::StaticClass());
	NewNodeAction->NodeTemplate = OpNode;
	ActionsBuilder.AddAction(NewNodeAction);

	ContextMenuBuilder.Append(ActionsBuilder);
}


void UEdGraphSchema_QuestTree::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const
{
	if (InGraphNode != nullptr)
	{
		MenuBuilder->BeginSection("GenericGraphAssetGraphSchemaNodeActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
		{
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Delete);
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Cut);
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Copy);
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Duplicate);

			MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
		}
		MenuBuilder->EndSection();
	}

	Super::GetContextMenuActions(CurrentGraph, InGraphNode, InGraphPin, MenuBuilder, bIsDebugging);
}


void UEdGraphSchema_QuestTree::GetAssetsGraphHoverMessage(const TArray<FAssetData>& Assets, const UEdGraph* HoverGraph, FString& OutTooltipText, bool& OutOkIcon) const
{
	UQuestTree* AssetQuestTree = FAssetData::GetFirstAsset<UQuestTree>(Assets);
	if (AssetQuestTree)
	{
		if (AssetQuestTree->ObjGraph == HoverGraph)
		{
			OutOkIcon = false;
			OutTooltipText = TEXT("Cannot drop quest tree to its own graph.");
		}
		else
		{
			//@todo check resurive reference
			OutOkIcon = true;
			OutTooltipText = TEXT("");
		}
	}
}


void UEdGraphSchema_QuestTree::DroppedAssetsOnGraph(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const
{
	UQuestTree* AssetQuestTree = FAssetData::GetFirstAsset<UQuestTree>(Assets);
	UQuestInstance* AssetQuestIntance = FAssetData::GetFirstAsset<UQuestInstance>(Assets);

	if (AssetQuestTree && AssetQuestTree->ObjGraph == Graph)
	{
		return;
	}

	if (AssetQuestIntance)
	{
		FEdQuestTreeSchemaAction_NewNode Action;
		UEdGraphNode_QuestInstance* OpNode = NewObject<UEdGraphNode_QuestInstance>(GetTransientPackage(), UEdGraphNode_QuestInstance::StaticClass());
		OpNode->QuestInstance = AssetQuestIntance;
		Action.NodeTemplate = OpNode;

		Action.PerformAction(Graph, nullptr, GraphPosition);
	}
	else if (AssetQuestTree)
	{
		FEdQuestTreeSchemaAction_NewNode Action;
		UEdGraphNode_QuestTree* OpNode = NewObject<UEdGraphNode_QuestTree>(GetTransientPackage(), UEdGraphNode_QuestTree::StaticClass());
		Action.NodeTemplate = OpNode;

		Action.PerformAction(Graph, nullptr, GraphPosition);
	}

}


class FConnectionDrawingPolicy* UEdGraphSchema_QuestTree::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	return new FConnectionDrawingPolicy_QuestTree(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

const FPinConnectionResponse UEdGraphSchema_QuestTree::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	// Make sure the pins are not on the same node
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Both are on the same node"));
	}

	// Compare the directions
	if ((A->Direction == EGPD_Input) && (B->Direction == EGPD_Input))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorInput", "Can't connect input node to input node"));
	}
	else if ((A->Direction == EGPD_Output) && (B->Direction == EGPD_Output))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorOutput", "Can't connect output node to output node"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
}


EGraphType UEdGraphSchema_QuestTree::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return EGraphType::GT_StateMachine;
}

#undef LOCTEXT_NAMESPACE