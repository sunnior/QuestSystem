#include "SGraphNode_QuestInstance.h"
#include "Slate.h"
#include "GraphEditorSettings.h"
#include "SCommentBubble.h"
#include "SLevelOfDetailBranchNode.h"
#include "TutorialMetaData.h"
#include "SGraphPin.h"
#include "EdGraphNode_QuestInstance.h"
#include "AssetEditorManager.h"

#include "QuestInstance.h"

class SGraphPin_QuestInstance : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SGraphPin_QuestInstance) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);

		bShowLabel = true;

		GraphPinObj = InPin;
		check(GraphPinObj != nullptr);

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);

		SBorder::Construct(SBorder::FArguments()
			.BorderImage(FEditorStyle::GetBrush(TEXT("AssetThumbnail.AssetBackground")))
			.BorderBackgroundColor(FLinearColor(0.02f, 0.02f, 0.02f))
			.OnMouseButtonDown(this, &SGraphPin_QuestInstance::OnPinMouseDown)
			//.Cursor(this, &SGenericGraphPin::GetPinCursor)
			.Padding(FMargin(7.0f))
		);
	}
};

void SGraphNode_QuestInstance::Construct(const FArguments& InArgs, UEdGraphNode_QuestInstance* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();

}

void SGraphNode_QuestInstance::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	NameAreaBox.Reset();

	GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
		.BorderBackgroundColor(FLinearColor(0.066f, 0.166f, 0.066f, 1.0f))
		.Padding(0.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.Padding(FMargin(0.0f, 0.0f, 0.0f, 4.0f))
		[
			SAssignNew(LeftNodeBox, SVerticalBox)
		]
	//Name Area
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(NameAreaBox, SVerticalBox)
		]

	+ SVerticalBox::Slot()
		.Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
		[
			SAssignNew(RightNodeBox, SVerticalBox)
		]
		]
		];

	CreateNameArea();
	CreatePinWidgets();
}

FReply SGraphNode_QuestInstance::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	UEdGraphNode_QuestInstance* NodeQuest = Cast<UEdGraphNode_QuestInstance>(GraphNode);
	FAssetEditorManager::Get().OpenEditorForAsset(NodeQuest->QuestInstance);

	return FReply::Handled();
}

void SGraphNode_QuestInstance::CreateNameArea()
{
	NameAreaBox->AddSlot()
		.AutoHeight()
		.Padding(FMargin(3.0f, 0.0f, 3.0f, 0.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SImage)
			.Image(FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Composite.Selector.Icon")))
		]
	+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Quest Instance")))
		.TextStyle(FEditorStyle::Get(), "Graph.Node.NodeTitle")
		]
		];

	NameAreaBox->AddSlot()
		.AutoHeight()
		.Padding(FMargin(3.0f, 0.0f, 3.0f, 0.0f))
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Collect gems as many as you can, Then sell them to the man, to buy some weapons.")))
		.WrapTextAt(240.0f)
		];
}


void SGraphNode_QuestInstance::CreatePinWidgets()
{
	UEdGraphNode_QuestInstance* StateNode = CastChecked<UEdGraphNode_QuestInstance>(GraphNode);

	for (int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SGraphPin_QuestInstance, MyPin)
				.ToolTipText(FText::FromString("Fuck Tool Tip"));

			AddPin(NewPin.ToSharedRef());
		}
	}
}


void SGraphNode_QuestInstance::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(60.0f, 0.0f)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{

		RightNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(60.0f, 0.0f)
			[
				PinToAdd
			];

		OutputPins.Add(PinToAdd);
	}
}
